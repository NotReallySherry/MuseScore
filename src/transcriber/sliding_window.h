#include <vector>
// define the sliding window class

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

class SlidingWindow {
    public:
        int size;
        int notes_count;
        
        std::vector<std::vector <int> > window;
        SlidingWindow(int window_size, int notes_per_window);

        void update(std::vector<int> new_notes);

        std::vector<int> get_notes();
};