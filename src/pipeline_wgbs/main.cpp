
#include "patter.h"
/***************************************************************
 *                                                             *
 *                     Main                                    *
 *                                                             *
 ***************************************************************/

/*
 * Input Arguments Parsering class
 */
class InputParser{
public:
    InputParser (int &argc, char **argv){
        for (int i=1; i < argc; ++i)
            this->tokens.emplace_back(std::string(argv[i]));
    }
    const std::string& getCmdOption(const std::string &option) const{
        std::vector<std::string>::const_iterator itr;
        itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
        if (itr != this->tokens.end() && ++itr != this->tokens.end()){
            return *itr;
        }
        static const std::string empty_string;
        return empty_string;
    }
    bool cmdOptionExists(const std::string &option) const{
        return std::find(this->tokens.begin(), this->tokens.end(), option)
               != this->tokens.end();
    }
private:
    std::vector <std::string> tokens;
};

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}


int main(int argc, char **argv) {
    clock_t begin = clock();
    try {
        InputParser input(argc, argv);
        int min_cpg = 1;
        if (input.cmdOptionExists("--min_cpg")){
            std::string min_cpg_string = input.getCmdOption("--min_cpg");
            if ( !is_number(min_cpg_string) )
                throw std::invalid_argument("Invalid min_cpg argument. Should be a non-negative integer.");
            min_cpg = std::stoi(min_cpg_string);
        }
        int clip = 0;
        if (input.cmdOptionExists("--clip")){
            std::string clip_str = input.getCmdOption("--clip");
            if ( !is_number(clip_str) )
                throw std::invalid_argument("Invalid clip argument. Should be a non-negative integer.");
            clip = std::stoi(clip_str);
        }
        float np_thresh = 0.67;
        if (input.cmdOptionExists("--np_thresh")){
            std::string np_thresh_str = input.getCmdOption("--np_thresh");
            np_thresh = std::stof(np_thresh_str);
        }
        if (argc < 3) {
            throw std::invalid_argument("Usage: patter CPG_DICT REGION [--mbias MBIAS_PATH] [--clip CLIP] [--nanopore] [--np_thresh NP_THRESH]");
        }
        std::string mbias_path = input.getCmdOption("--mbias");
        bool is_np = input.cmdOptionExists("--nanopore");
        patter p(argv[1], argv[2], mbias_path, min_cpg, clip, is_np, np_thresh);
        p.parse_reads_from_stdin();

    }
    catch (std::exception &e) {
        std::cerr << "[ patter ] Failed! exception:" << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
