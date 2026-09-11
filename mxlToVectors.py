from music21 import converter
import pretty_midi
import sys
import fluidsynth
import numpy as np

# template file in cpp calls this file with xml filepath as arg
if len(sys.argv) != 2:
    raise ValueError("Expected MusicXML path")
music_xml_path = sys.argv[1]

# music xml to music21 score
score = converter.parse(music_xml_path)
midi_path = music_xml_path[:-4] + ".mid"

# music 21 to midi file
score.write('midi', fp=midi_path)



# could make for an interesting contribution to music21 itself cause they're pedalmarks to midiCC isn't cleaning working
# making chatgpt deal with the lack of pedal output and coding up a quick workaround (we love vibecoding)
from pretty_midi import ControlChange

pm = pretty_midi.PrettyMIDI(midi_path)
pianos = [i for i in pm.instruments if not i.is_drum]

for pedal in score.recurse().getElementsByClass("PedalMark"):
    first = pedal.getFirst()
    last = pedal.getLast()

    start_offset = first.getOffsetInHierarchy(score)
    end_offset = last.getOffsetInHierarchy(score)

    # Convert music21 quarter-note offsets -> MIDI ticks
    start_tick = round(start_offset * pm.resolution)
    end_tick = round(end_offset * pm.resolution)

    # Let PrettyMIDI's actual MIDI tempo map determine the time
    start_seconds = pm.tick_to_time(start_tick)
    end_seconds = pm.tick_to_time(end_tick)

    for piano in pianos:
        piano.control_changes.append(
            ControlChange(number=64, value=127, time=start_seconds)
        )

        piano.control_changes.append(
            ControlChange(number=64, value=0, time=end_seconds)
        )

for piano in pianos:
    piano.control_changes.sort(key=lambda cc: cc.time)

pm.write(midi_path)



# midi file to audio vector 

audio = pm.fluidsynth(fs=24000, synthesizer="data_files/piano.sf2")
audio = np.asarray(audio).astype(np.float32)
audio_path = music_xml_path[:-4] + "audio.f32"

# downbeat indices
downbeats_in_sec = pm.get_downbeats()
downbeat_samples = np.rint(downbeats_in_sec * 24000).astype(np.int64)

measures = list(score.parts[0].getElementsByClass("Measure"))

measure_indices = np.array(
    [measure.number for measure in measures[:len(downbeat_samples)]],
    dtype=np.int64
)

# downbeats are offset by anacrusis. i.e. in a 4/4 piece, if there is 2 beat anacrusis, then the start of piece
# is the first downbeat, and then each downbeat is offset by 2 beats. 
# could be due to xml to midi conversion, or prettymidi itself, but its a problem for later cause tracking 
# should still work. 
downbeats = np.empty(len(downbeat_samples) * 2, dtype=np.int64)
downbeats[0::2] = downbeat_samples
downbeats[1::2] = measure_indices
downbeats_path = music_xml_path[:-4] + "downbeats.i64"

# write to 'data_files'
audio.tofile(audio_path)
downbeats.tofile(downbeats_path)


# for debugging/listening purposes
import soundfile as sf
sf.write("data_files/test.wav", audio, 24000)

