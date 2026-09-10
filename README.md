# LookAhead
writing an automated page turner from scratch in c++ 20 to learn about real-time audio/systems programming!

goal is to use pitch binning (cqt) with HMM/OLTW based approaches (documented in literature) to detect position on sheet music while user plays a piece.    
this positional information would be used to trigger the half-page turns I need to make this work!

engine will be written from scratch. musicXML parsing and rendering will be done using existing libraries.

# tasks:
1. ~~use miniaudio and get audio input working~~
2. ~~pitch detection logic to detect a440 on mic input~~
3. ~~real-time pitch detection (naive-cqt) across 88 piano pitches.~~
4. ~~visualizer for pitch bins (good for quick reference)~~
5. ~~structure to build 24khz audio sample vector from xml file (use python libs)~~
6. ~~build template from audio sample vector and testttt~~
7. HMM model to track input audio (from computer or live mic) against templates. 


# notes:
- i'm starting to see why naming conventions, and clear architecture drawings are needed to maintain and use clean code lol


# future explorations:
1. optimize naive cqt with parallelism. 
2. build musicXML to WAV from scratch (or rather, musicXML to trackable templates)
 

# acknowledgements/references/dependencies:
1. oltw paper by dixon
2. real-time audio programming 101: time waits for nothing by ross bencina
3. miniaudio 
4. https://freepats.zenvoid.org/Piano/acoustic-grand-piano.html piano sound font
5. pretty\_midi and music21 and fluidsynth

