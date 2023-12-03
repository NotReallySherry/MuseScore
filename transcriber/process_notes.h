#include "sliding_window.h"
// wrapper around the sliding window class
// takes in raw data from the rasperry pi and returns processed notes

class ProcessNotes {
    private:
        SlidingWindow window;
        int polling_speed;
        int whole_note_length;
        std::ofstream& outfile;

    public:
        ProcessNotes(int window_size, int notes_per_window, int p_speed, int whole_note_length, std::ofstream& output_file);

        void update(std::string new_notes);
        std::vector<std::pair<int, int> > get_notes(); // output: vector of pairs of (duration, average_dynamics)

        void print_get_notes_result(std::vector<std::pair<int, int> > result);
        // void call_api(std::vector<std::pair<int, int> > result); 

        void print_result_terminal_helper(int note, int length, int dynamics);
        int round_to_power_of_two(int num);
        std::string print_result_terminal_helper_note(int note);
        std::string print_result_terminal_helper_length(int note_length_musically);
        std::string print_result_terminal_helper_dynamics(int dynamics);

        int get_polling_speed();
};