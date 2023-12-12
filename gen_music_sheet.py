
import abjad
 
if __name__ == "__main__":
    with open("cppoutput.txt", "r") as f:
        string = f.read()
        
        
    time_signature = abjad.TimeSignature((3, 4))

    voice = abjad.Voice(string, name="Voice")
    staff = abjad.Staff([voice], name="Staff")
    articulation = abjad.Articulation("turn")
    abjad.attach(time_signature, voice[0])
    abjad.attach(articulation, voice[5])
    score = abjad.Score([staff], name="Score")

    abjad.show(score)