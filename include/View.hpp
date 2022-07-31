#ifndef View_hpp
#define View_hpp

#include "Bibliotecas.hpp"

class View{

    private:
        int m_score; //!<score atual
        int m_food_eat; //!<Comidas recolhidas no Level
        int m_food_all; //!<Total de comidas a colher
        int m_lives; //!<Vidas restantes
        std::vector<std::vector<int>> m_maze_assembly_debug; //!Cópia da matriz em assembly que possui atualizações da posição atual do player
    public:

    /**
     * @brief Construct que apresenta a inicialização da View
     * 
     */
    View();

    /**
     * @brief Método para visualizar a interface gráfica do labirinto
     * @param maze_map, o mapa atual
     * @param char, a cabeça do pacman virada para a frente (depende da sua direção)
     */
    void view_maze(std::vector<std::vector<int>> maze_map,char);
    /**
     * @brief Método para visualizar a interface gráfica do labirinto com a movimentação
     * do pacman/snaze por ele
     * @param maze_map, o mapa atual
     * @param char, a cabeça do pacman virada para a frente (depende da sua direção)
     * @param int, move, um número que representa o movimento do pacman: UP,DOWN,LEFT,RIGHT ou STAY
     */
    void view_maze_debug(std::vector<std::vector<int>> maze_map,char,int,std::pair<int,int>);
    /**
     * @brief Método para visualizar a interface gráfica do cabeçalho do game
     * 
     */
    void view_header(std::vector<std::vector<char>> header);

    /**
     * @brief Visualização da tela Game Over ao final do jogo
     * 
     */
    void view_game_over();

    /**
     * @brief Atualização das informações do cabeçalho
     * 
     * @param score 
     * @param food_all 
     * @param food_eat 
     * @param lives 
     */
    void update_info_header(int score, int food_all, int food_eat, int lives);

    void initialize_maze_assembly_debug(int l,int c);

    void view_game_over(std::vector<std::string> game_over);

};

#endif