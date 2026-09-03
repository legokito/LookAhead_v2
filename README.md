writing an automated page turner from scratch in c++ 20 to learn about real-time audio/systems programming!

tasks:
1. ~~use miniaudio and get audio input working~~
2. ~~pitch detection logic to detect a440 on mic input~~
3. ~~real-time pitch detection (naive-cqt) across 88 piano pitches.~~
4. visualizer for pitch bins (good for quick reference)
5. map live playing chroma model to synthesized chroma model of same piece of music. (will probably be split into more tasks, but robustly verifying this gives confirmation on whether this project could feasibly work).

goal is to use pitch binning (cqt) with HMM/OLTW based approaches (documented in literature) to detect position on sheet music while user plays a piece.    
this positional information would be used to trigger the half-page turns I need to make this work!

engine will be written from scratch. musicXML parsing and rendering will be done using existing libraries.

future explorations:
1. optimize naive cqt with parallelism, etc. -> eventually replace with learned model
2. 
 

