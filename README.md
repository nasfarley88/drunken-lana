drunken-lana
============

Basic music player designed to use a transcription pedal.

Compile with

    g++ main.cxx window.cxx -o main `pkg-config gtkmm-3.0 libvlc --cflags --libs`

Note that 'pause' in this program stops the audio at the current point only. Pressing pause again will have no effect; press play to start the audio again.