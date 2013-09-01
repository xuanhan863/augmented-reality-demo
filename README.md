# Augmented Reality Demo

This little program will detect a 5×5 checkerboard pattern and projects an image into it.

## Prequisits

* OpenCV for computer vision (`brew install opencv`)
* Boost for multi-threading (`brew install boost`)

## Build

```sh
cmake .
make
```

## Usage
```sh
./track <IMAGE PATH>
```

Currently, streaming the camera and detecting the pattern run in their own
threads.  This provides the benefit of a smooth video stream, whereas it
normally gets choppy in cases where the pattern is not found (presumably as the
algorithm exhaustively searches).

The downside to this it that the detection is usually "behind" where the stream
is and this it appears that the projection is chasing after where it should be.

Without using threads, the projection is usually dead-on, but the video is
usually less performant.
