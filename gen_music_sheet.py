
import abjad
 
if __name__ == "__main__":
    with open("cppoutput.txt", "r") as f:
        string = f.read()
        
    voice = abjad.Voice(string, name="Voice")
    staff = abjad.Staff([voice], name="Staff")
    
    abjad.show(staff)