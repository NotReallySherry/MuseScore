#include "sliding_window.h"
// wrapper around the sliding window class
// takes in raw data from the rasperry pi and returns processed notes

class ProcessNotes {
    private:
        SlidingWindow window;
        int polling_speed;

    public:
        ProcessNotes(int window_size, int notes_per_window, int p_speed);

        void update(std::string new_notes);
        std::vector<std::pair<int, int> > get_notes(); // output: vector of pairs of (duration, average_dynamics)

        void print_get_notes_result(std::vector<std::pair<int, int> > result);

        int get_polling_speed();
};