
// define a output struct

#include <string>
enum noteDuration {
    whole = 1,
    half = 2,
    quarter = 3,
    eighth = 4,
    sixteenth = 5,
    thirtysecond = 6,
    sixtyfourth = 7
};

struct processedNotes {
    int noteNumber;
    noteDuration duration;
};

std::vector<processedNotes> process_notes(std::string notes) {
    std::vector<processedNotes> result;
    std::string note;
    std::string duration;
    std::string delimiter = " ";
    size_t pos = 0;
    std::string token;
    while ((pos = notes.find(delimiter)) != std::string::npos) {
        token = notes.substr(0, pos);
        note = token.substr(0, 1);
        duration = token.substr(1, 1);
        processedNotes temp;
        temp.noteNumber = std::stoi(note);
        temp.duration = static_cast<noteDuration>(std::stoi(duration));
        result.push_back(temp);
        notes.erase(0, pos + delimiter.length());
    }
    note = notes.substr(0, 1);
    duration = notes.substr(1, 1);
    processedNotes temp;
    temp.noteNumber = std::stoi(note);
    temp.duration = static_cast<noteDuration>(std::stoi(duration));
    result.push_back(temp);
    return result;
}