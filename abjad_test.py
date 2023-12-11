
import abjad

string1 = "<a d'>8 f' a' d'' f'' gs'4 r8 e' gs' b' e'' gs'' a'4 "
voice1 = abjad.Voice(string1, name="RH_Voice")
staff1 = abjad.Staff([voice1], name="RH_Staff")
abjad.show(staff1)
