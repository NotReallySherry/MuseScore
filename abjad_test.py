
import abjad

string = "<g' >32 <f' >16  <d' as' >16 <b' >32 <d' e' f' a' >32 <e' >1 <d' >1 <b' >1 <a' >1 "

time_signature = abjad.TimeSignature((3, 4))

voice = abjad.Voice(string, name="Voice")
staff = abjad.Staff([voice], name="Staff")
articulation = abjad.Articulation("turn")
abjad.attach(time_signature, voice[0])
abjad.attach(articulation, voice[5])
score = abjad.Score([staff], name="Score")

abjad.show(score)