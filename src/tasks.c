#include "tasks.h"
#include "interpolators.h"

#include "raymath.h"

Task_VTable task_vtable = {0};
Tag TASK_MOVE_SCALAR_TAG = 0;
Tag TASK_MOVE_VEC2_TAG = 0;
Tag TASK_MOVE_VEC4_TAG = 0;
Tag TASK_SEQ_TAG = 0;
Tag TASK_GROUP_TAG = 0;
Tag TASK_WAIT_TAG = 0;

bool task_update(Task task, Env env) {
    return task_vtable.items[task.tag].update(task.data, env);
}

Tag task_vtable_register(Arena *a, Task_Funcs funcs) {
    Tag tag = task_vtable.count;
    arena_da_append(a, &task_vtable, funcs);
    return tag;
}

void task_vtable_rebuild(Arena *a) {
    memset(&task_vtable, 0, sizeof(task_vtable));

    TASK_MOVE_SCALAR_TAG = task_vtable_register(a, (Task_Funcs) {
        .update = (task_update_data_t)task_move_scalar_update,
    });
    TASK_MOVE_VEC2_TAG = task_vtable_register(a, (Task_Funcs) {
        .update = (task_update_data_t)task_move_vec2_update,
    });
    TASK_MOVE_VEC4_TAG = task_vtable_register(a, (Task_Funcs) {
        .update = (task_update_data_t)task_move_vec4_update,
    });
    TASK_SEQ_TAG = task_vtable_register(a, (Task_Funcs) {
        .update = (task_update_data_t)task_seq_update,
    });
    TASK_GROUP_TAG = task_vtable_register(a, (Task_Funcs) {
        .update = (task_update_data_t)task_group_update,
    });
    TASK_WAIT_TAG = task_vtable_register(a, (Task_Funcs) {
        .update = (task_update_data_t)task_wait_update,
    });
}

bool task_move_scalar_update(Move_Scalar_Data *data, Env env) {
    if (data->t >= 1.0f) return true; // task is done

    if (!data->init) {
        // First update of the task
        if (data->value) data->start = *data->value;
        data->init = true;
    }

    data->t = (data->t*data->duration + env.delta_time)/data->duration;
    if (data->value) *data->value = Lerp(data->start, data->target, smoothstep(data->t));
    return data->t >= 1.0f;
}

Task task_move_scalar(Arena *a, float *value, float target, float duration) {
    Move_Scalar_Data *data = arena_alloc(a, sizeof(*data));
    memset(data, 0, sizeof(*data));
    data->value = value;
    data->target = target;
    data->duration = duration;
    return (Task) {
        .tag = TASK_MOVE_SCALAR_TAG,
        .data = data,
    };
}

bool task_move_vec2_update(Move_Vec2_Data *data, Env env) {
    if (data->t >= 1.0f) return true; // task is done

    if (!data->init) {
        // First update of the task
        if (data->value) data->start = *data->value;
        data->init = true;
    }

    data->t = (data->t*data->duration + env.delta_time)/data->duration;
    if (data->value) *data->value = Vector2Lerp(data->start, data->target, smoothstep(data->t));
    return data->t >= 1.0f;
}

Task task_move_vec2(Arena *a, Vector2 *value, Vector2 target, float duration) {
    Move_Vec2_Data *data = arena_alloc(a, sizeof(*data));
    memset(data, 0, sizeof(*data));
    data->value = value;
    data->target = target;
    data->duration = duration;
    return (Task) {
        .tag = TASK_MOVE_VEC2_TAG,
        .data = data,
    };
}

bool task_move_vec4_update(Move_Vec4_Data *data, Env env) {
    if (data->t >= 1.0f) return true;

    if (!data->init) {
        // First update of the task
        if (data->value) data->start = *data->value;
        data->init = true;
    }

    data->t = (data->t*data->duration + env.delta_time)/data->duration;
    if (data->value) *data->value = QuaternionLerp(data->start, data->target, smoothstep(data->t));
    return data->t >= 1.0f;
}

Task task_move_vec4(Arena *a, Vector4 *value, Color target, float duration) {
    Move_Vec4_Data *data = arena_alloc(a, sizeof(*data));
    memset(data, 0, sizeof(*data));
    data->value = value;
    data->target = ColorNormalize(target);
    data->duration = duration;
    return (Task) {
        .tag = TASK_MOVE_VEC4_TAG,
        .data = data,
    };
}

bool task_group_update(Group_Data *data, Env env) {
    bool finished = true;
    for (size_t i = 0; i < data->tasks.count; ++i) {
        Task it = data->tasks.items[i];
        if (!task_update(it, env)) {
            finished = false;
        }
    }
    return finished;
}

Task task_group_(Arena *a, ...) {
    Group_Data *data = arena_alloc(a, sizeof(*data));
    memset(data, 0, sizeof(*data));

    va_list args;
    va_start(args, a);
    for (;;) {
        Task task = va_arg(args, Task);
        if (task.data == NULL) break;
        arena_da_append(a, &data->tasks, task);
    }
    va_end(args);

    return (Task) {
        .tag = TASK_GROUP_TAG,
        .data = data,
    };
}

bool task_seq_update(Seq_Data *data, Env env) {
    if (data->it >= data->tasks.count) return true;

    Task it = data->tasks.items[data->it];
    if (task_update(it, env)) {
        data->it += 1;
    }

    return data->it >= data->tasks.count;
}

Task task_seq_(Arena *a, ...) {
    Seq_Data *data = arena_alloc(a, sizeof(*data));
    memset(data, 0, sizeof(*data));

    va_list args;
    va_start(args, a);
    for (;;) {
        Task task = va_arg(args, Task);
        if (task.data == NULL) break;
        arena_da_append(a, &data->tasks, task);
    }
    va_end(args);

    return (Task) {
        .tag = TASK_SEQ_TAG,
        .data = data,
    };
}

bool task_wait_update(Wait_Data *data, Env env) {
    if (data->t >= data->duration) return true;
    data->t += env.delta_time;
    return data->t >= data->duration;
}

Task task_wait(Arena *a, float duration) {
    Wait_Data *data = arena_alloc(a, sizeof(*data));
    memset(data, 0, sizeof(*data));
    data->duration = duration;
    return (Task) {
        .tag = TASK_WAIT_TAG,
        .data = data,
    };
}
