#ifndef Level_hpp
#define Level_hpp

#include "Bibliotecas.hpp"

using namespace std;

/*
    - Modela um mapa do jogo
    - Pode ser usado para testar posições ou desenhar o mapa

*/

class Level{
    private:
        static int _level;//!Atributo estático que consta o nível atual
        string m_name_file; //!<Caminho do arquivo onde está o mapa

        int m_size_line; //!<Quantidade de linhas dos mapas

        int m_size_col;  //!<Quantidade de colunas dos mapas

        std::pair<int, int> m_pacman_position_found; //!<Posição encontrada do pacman

        bool m_status_pacman = false;

        std::vector<std::pair<int,int>> m_coord_free; //!<Caminhos livres do mapa

        std::vector<std::string> m_container_map; //!<O mapa em sua versão vetor

        std::vector<std::vector<int>> m_maze_map; //!< labirinto atual em 0 e 1

        std::vector<int> m_positions_food_level; //!<Containers de posições das foods no level (Devem ser geradas pela ramdom)

        int m_amount_foods; //!<Quantidade de comidas que o Level gerará

        std::vector<std::pair<int, int>> m_coords_food; //!<Coordenadas das comidinhas

        bool status_pacman_maze = true; //!<Indica se ele bateu ou não na barreira

        int m_round_atual = 0; //!Indica qual round o jogador está

    public:
        /**
         * @brief Construct a new Level object
         * 
         */
        Level();

        /**
         * @brief Construct a new Level object
         * 
         * @param name_file 
         * @param size_line
         * @param size_col
         */
        Level(string name_file, int size_line, int size_col, int amount_food);

        /**
         * @brief Atualização do estado do mapa
         * 
         * @param pair_position_pacman 
         * @param pair_position_food 
         * @param rastro, True se for debug ele salva o rastro, False se for find normal
         * @return std::vector<std::vector<int>> 
         */
        std::vector<std::vector<int>> update_assembler_maze(std::pair<int, int> position_pacman, std::pair<int, int> position_food,bool rastro,int);

        /**
         * @brief Método para processar mapas o mapa
         * @return std::vector<std::vector<int>> 
         */
        std::vector<std::vector<int>> get_process_map();

        /**
         * @brief Retorna o status do pacman no mapa
         * 
         * @return true 
         * @return false 
         */
        bool get_status_pacman();

        /**
         * @brief Onde o Pacman foi encontrado dentro do mapa processado
         * 
         * @return std::pair<int, int> 
         */
        std::pair<int, int> get_pacman_position_found();

        /**
         * @brief Container com todos os caminhos livres pelo mapa
         * 
         * @return std::vector<std::pair<int,int>> 
         */
        std::vector<std::pair<int,int>> get_coords_free();

        /**
         * @brief Processamento bruto do mapa
         * 
         */
        void process_map();

        /**
         * @brief Estado onde identifica se o pacman comeu todos os foods do level, caso sim, deve finalizar o level
         * 
         * @return true 
         * @return false 
         */
        bool end_of_level(int amount_delicious);

        /**
         * @brief Set the coords food object
         * 
         * @param c_free 
         */
        void set_coords_food(std::pair<int,int> c_free);

        std::vector<std::pair<int, int>> get_coords_food();

        /**
         * @brief Retorna se ele bateu ou não na barreira
         * 
         * @return true 
         * @return false 
         */
        bool status_player_maze();

        /**
         * @brief Reiniciando a simulação do nível
         * 
         */
        void restart_level();

        /**
         * @brief Incrementa calculo do scores recolhido
         * 
         * @param number_of_steps -> Número de passos
         * @return float 
         */
        float increment_score(int number_of_steps);

        /**
         * @brief Resetar comidinhas disponíveis
         * 
         */
        void reset_food_dispo();
        /**
         * @brief Reseta o round ao mudar de nível
         * 
         */
        void reset_round();
        /**
         * @brief Resgata a última posição do pacman no nível anterior.
         * 
         * @param pac_pos, recebe a última posição do pacman
         */
        void set_pacman_position_found_new_level(std::pair<int,int> pac_pos);

};

#endif