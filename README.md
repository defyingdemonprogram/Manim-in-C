## Manim in C (Panim)

This project is inspired by the Manim implementation from 3b1b, and the code is developed as part of a C programming learning journey, based on @tsoding's YouTube tutorials.

### Quick Start
1. Build the project:
```bash
cc -o nob nob.c  # Run this only once
./nob
```

1. Running the Project
```bash
./build/panim ./build/libtm.so
```

**Major Hotkeys for the Program**:
- <kbd>R</kbd>: Render the video from the current Manim frame
- <kbd>T</kbd>: Render the sound from the current Manim frame
- <kbd>H</kbd>: Hot reload the program
- <kbd>A</kbd>: Restart the animation
- <kbd>ESC</kbd> or <kbd>Q</kbd>: Exit the program

### Architecture

The engine is composed of two main components:

1. **Panim Executable** – This is the core engine.
2. **Animation Dynamic Library** – Also known as `libtm.so`, this is the library that the user of Panim develops.

The **Panim Executable** allows you to control your animation: pause, replay, and, most importantly, render it into a final video using FFmpeg. It also offers the ability to dynamically reload the animation library without restarting the entire engine, which significantly improves the development feedback loop.

#### Assets and State

When developing your animation dynamic library, it's useful to separate your elements into two distinct lifetimes:

1. **Assets**: These are elements that remain unchanged throughout the animation but are reloaded whenever the `libplug.so` is reloaded.
2. **State**: These are elements that persist across a `libplug.so` reload but are reset when the `plug_reset()` function is called.


#### References
- [Easing Function](https://easings.net/)