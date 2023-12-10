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

class SlidingWindow {
    public:
        int size;
        int notes_count;
        int capacity;
        
        std::vector<std::vector <int> > window;
        SlidingWindow(int window_capacity, int notes_per_window);

        void update(std::vector<int> new_notes);

    std::vector<std::pair<int, int> > get_notes();

        // helper function
        void print_window();
};