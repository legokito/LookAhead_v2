writing an automated page turner from scratch in c++ 20 to learn about real-time audio/systems programming!

tasks:
1. ~~use miniaudio and get audio input working~~
2. pitch detection logic to detect a440 on mic input
3. real-time pitch detection (naive-cqt) across 88 piano pitches.

goal is to use pitch binning (cqt) with HMM/OLTW based approaches (documented in literature) to detect position on sheet music while user plays a piece.    
this positional information would be used to trigger the half-page turns I need to make this work!

future explorations:
1. optimize naive cqt with parallelism, etc. -> eventually replace with learned model
2. 
 

