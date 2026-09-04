writing an automated page turner from scratch in c++ 20 to learn about real-time audio/systems programming!

goal is to use pitch binning (cqt) with HMM/OLTW based approaches (documented in literature) to detect position on sheet music while user plays a piece.    
this positional information would be used to trigger the half-page turns I need to make this work!

engine will be written from scratch. musicXML parsing and rendering will be done using existing libraries.

tasks:
1. ~~use miniaudio and get audio input working~~
2. ~~pitch detection logic to detect a440 on mic input~~
3. ~~real-time pitch detection (naive-cqt) across 88 piano pitches.~~
4. ~~visualizer for pitch bins (good for quick reference)~~

current question: will the tracking reliably work???   
current idea:
- feed a reference recording (generated from musicXML -> WAV), and build HMM based model to track live audio stream against that reference recording. 
(if this tracking reliably works, building the infrastructure around is purely an engineering challenge rather than an unknown that blocks this project from working. ).

5. structure to build templates from WAV + midi data
6. HMM model to track input audio (from computer or live mic) against templates. 



future explorations:
1. optimize naive cqt with parallelism. 
2. build musicXML to WAV from scratch (or rather, musicXML to trackable templates)
 

