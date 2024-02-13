<img style="position:relative;margin:0px;padding:0;top:40px" src="https://i.imgur.com/cCT9KwN.png" width="104"/>
<h1 style="margin-top:0px;padding-top:0px">Louvre Tutorial</h1>

<p align="left">
  <a href="https://github.com/CuarzoSoftware/Louvre/blob/main/LICENSE">
    <img src="https://img.shields.io/badge/license-MIT-blue.svg" alt="Louvre is released under the MIT license." />
  </a>
</p>

This repository hosts the source code of the final result achieved upon completing the [Louvre Tutorial](https://cuarzosoftware.github.io/Louvre/md_md_tutorial_01.html).

<img src="https://lh3.googleusercontent.com/pw/ADCreHfr83RGTD9to6OLt-ohbjFw4bYgJJS9H8UunucxoCBwB5_m8nHOV_LXzIWObyhAk3xGLPpFfLS2YTiRTiTOFD2QdhyQYPm8KtJvb6VQlhHhgexhA1M=w2400"/>

## Building

```bash
cd LouvreTutorial/
meson setup build
cd build
meson compile
```

## Shortcuts

- Press `Ctrl + Shift + Esc` to exit.
- Press `F1` to launch *weston-terminal*.
- Press `Super + Shift + [Up/Down]` to increase/decrease the scaling factor by 0.25.

## Running

Switch to a free TTY by pressing `Ctrl + Alt + [F1, F2, ..., FN]` or by using the `chvt` command, then execute:

```bash
cd LouvreTutorial/build
./louvre-example
```