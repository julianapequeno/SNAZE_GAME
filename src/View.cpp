#include "View.hpp"

View::View(){
   
}

void View::view_maze(std::vector<std::vector<int>> maze_map,char pac){
    for(int l = 0; l < maze_map.size(); l++){
        for(int c = 0; c < maze_map[0].size(); c++){
            if(maze_map.at(l).at(c) == 0){
                std::cout<<BG_GREEN ANSI_COLOR_BLACK "´" ANSI_COLOR_RESET BG_RESET;
            } else if(maze_map.at(l).at(c) == 1){
                std::cout<< ANSI_COLOR_BLACK " " ANSI_COLOR_RESET ;
            } else if(maze_map.at(l).at(c) == 4){
                std::cout<< ANSI_COLOR_RED "*" ANSI_COLOR_RESET;
            } else {
                std::cout<< ANSI_COLOR_YELLOW << pac <<ANSI_COLOR_RESET; 
            }
        }
        std::cout<<"\n";
    }
}

void View::view_maze_debug(std::vector<std::vector<int>> maze_map,char pac, int last_move,std::pair<int,int> pos_pac){

    for(int l = 0; l < maze_map.size(); l++){
        for(int c = 0; c < maze_map[0].size(); c++){
            if(maze_map.at(l).at(c) == -1){
                std::cout<<BG_RED ANSI_COLOR_BLACK "´" ANSI_COLOR_RESET BG_RESET;
            }else if(maze_map.at(l).at(c) == 0){
                std::cout<<BG_GREEN ANSI_COLOR_BLACK "´" ANSI_COLOR_RESET BG_RESET;
            } else if(maze_map.at(l).at(c) == 1){
                std::cout<< ANSI_COLOR_BLACK " " ANSI_COLOR_RESET ;
            } else if(maze_map.at(l).at(c) == 4){
                std::cout<< ANSI_COLOR_RED "*" ANSI_COLOR_RESET;
            }else if(maze_map.at(l).at(c) == 11){//UP
                std::cout<< ANSI_COLOR_YELLOW << "^" <<ANSI_COLOR_RESET; 
            }else if(maze_map.at(l).at(c) == 12){//DOWN
                std::cout<< ANSI_COLOR_YELLOW << "v" <<ANSI_COLOR_RESET; 
            }else if(maze_map.at(l).at(c) == 13){//LEFT
                std::cout<< ANSI_COLOR_YELLOW << "<" <<ANSI_COLOR_RESET; 
            }else if(maze_map.at(l).at(c) == 14){//RIGHT
                std::cout<< ANSI_COLOR_YELLOW << ">" <<ANSI_COLOR_RESET; 
            }else if(maze_map.at(l).at(c) == 3){
                std::cout<< ANSI_COLOR_YELLOW << pac <<ANSI_COLOR_RESET; 
            }
        }
        std::cout<<"\n";
    }
}

void View::view_header(std::vector<std::vector<char>> header){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < header[0].size(); j++){
            if(header.at(i).at(j) != 'K'){
                if(header.at(i).at(j) == '*'){
                    std::cout<< BG_BLACK ANSI_COLOR_RED << m_lives << ANSI_COLOR_RESET BG_RESET;
                } else 
                if(header.at(i).at(j) == '-'){ //Identificando campo scores
                    std::cout<< BG_BLACK ANSI_COLOR_BLUE<< m_score <<ANSI_COLOR_RESET BG_RESET;
                } else 
                if(header.at(i).at(j) == '<'){ //Identificando campo fome atual
                    std::cout<< BG_BLACK ANSI_COLOR_MAGENTA << m_food_eat  << ANSI_COLOR_RESET BG_RESET;
                } else
                if(header.at(i).at(j) == '>'){ //Identificando campo total fome
                    std::cout<< BG_BLACK ANSI_COLOR_MAGENTA << m_food_all + 1  << ANSI_COLOR_RESET BG_RESET;
                } else 
                if(header.at(i).at(j) == 'X'){
                    std::cout<< BG_BLACK " " BG_RESET;
                } else {
                    std::cout<< BG_BLACK ANSI_COLOR_YELLOW <<header.at(i).at(j)<< ANSI_COLOR_RESET BG_RESET;
                }
            } else 
                if(header.at(i).at(j) == 'K'){
                    std::cout<< BG_BLACK ANSI_COLOR_BLUE "+" ANSI_COLOR_RESET BG_RESET;
                }
        }
        std::cout<<"\n";
    }
}

void View::view_game_over(){
     
}

void View::update_info_header(int score, int food_all, int food_eat, int lives){
    m_score = score;
    m_food_eat = food_eat; 
    m_food_all = food_all; 
    m_lives = lives; 
}

void View::initialize_maze_assembly_debug(int l,int c){
    this->m_maze_assembly_debug = std::vector< std::vector<int> > (l, std::vector<int>(c));
}

void View::view_game_over(std::vector<std::string> game_over){
    for(std::string l : game_over){
        for(char c : l){
            if(c == 'X'){
                std::cout<< BG_YELLOW " " BG_RESET;
            } else
                if(c == 'z'){
                    std::cout<< BG_RED " " BG_RESET;
            } else {
                std::cout<< BG_BLACK " " BG_RESET;
            }
        }
        std::cout<<"\n";
    }
}
