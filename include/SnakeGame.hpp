#ifndef SnakeGame_hpp
#define SnakeGame_hpp

#include "Player.hpp"
#include "Pacman.hpp"
#include "Level.hpp"
#include "Bibliotecas.hpp"
#include "View.hpp"
#include "cstdlib"

class SnakeGame{
    public:
        /**
        * @brief descreve os possíveis estados do jogo, fique à vontade para adicionar outros se necessário
        **/
        enum GameStates{
            RUNNING, //<! quando o jogo está executando o estado é RUNNING
            GAME_OVER, //<! quando o jogo deve terminar o estado é GAME_OVER
            WAITING_USER, //<! quando o jogo deve esperar por uma entrada do usuário o estado é WAITING_USER
            WAITING_IA //<! usualmente o jogo está esperando por comandos da IA, neste caso o estado é WAITING_IA
        };

    private:
        Player m_ia_player; //<! instancia da classe Player responsável pela IA do jogo

        std::vector<Level> m_level; //<! Container de instancias dos Levels 

        Pacman *m_pacman; //<! instancia do pacman

        View m_view; //<! instancia dos ativadores do Frontend do game

        int m_current_level; //<! Um contador que indica o level atual

        std::vector<std::vector<int>> m_maze_assembly; //! Vector do labirinto atual em 0 e 1

        int m_frameCount; //<! contador de frames, usado apenas como exemplo

        std::string m_choice; //<! usado na função process_actions para guardar a escolha do usuário

        GameStates m_state; //<! guarda o estado do jogo

        std::string m_levels_file; //<! arquivo com os níveis do jogo

        int m_action = 0; //<! Representa a ação escolhida pela IA -  Começa em 0 pelo STAY

        std::vector<std::string> m_maze_header; //!< Vetor do cabeçalho

        std::vector<std::vector<char>> m_header; //!< Matriz do cabeçalho

        std::vector<int> values_map; //!< Os três valores da primeira linha do mapa

        std::pair<int,int> m_posicao_food;

        float m_score; //!<Scores do game atual

        int m_step_counter; //<!Quantidade de passos do jogador

        std::vector<std::string> m_game_over; //!<Container dos dados de Game Over

        bool status_level_game=true; //Status do level atual no game, verificando se acabou comidas

        static std::minstd_rand rnd;
        
        /**
         * @brief consta que o pacmaze que está sendo jogado
         */
        bool PACMAZE = false;
        /**
         * @brief consta que a snaze que está sendo jogada
         */
        bool SNAZE = false;
        /**
         * @brief consta o modo em que o jogo está rodando;
         * true, se está rodando no modo FIND
         * false, se está rodando no modo RANDOM
         */
        bool MODE;
        /**
         * @brief consta que o modo debug do pacmaze está sendo jogado
         */
        bool PACMAZE_DEBUG = false;
    public:
        /**
        * @brief construtor padrão
        **/
        SnakeGame(std::string levels, std::string mode, std::string ia, bool debug);

        /**
        * @brief chamado no main, este loop executa o jogo indefinidamente até que o usuário escolha terminar!
        */
        void loop();
    private:
        /**
        * @brief realiza a inicialização geral do jogo, fique à vontade para adicionar parâmetros se achar que deve
        **/
        void initialize_game();

        /**
        * @brief atualiza o estado do jogo, basicamente movendo os objetos, sorteando novas posições de comida
        **/
        void update();

        /**
        * @brief processa as entradas do jogador
        **/
        void process_actions();

        /**
        * @brief testa o estado do jogo e desenha a tela correspondente
        **/
        void render();

        /**
        * @brief é chamada quando o jogo termina a fim de destruir/resetar elementos do estado do jogo
        **/
        void game_over();

        /**
         * @brief Incrementar header no escopo da classe
         * 
         */
        void set_header();

        /**
         * @brief Sorteio aleatorio de coordenada para a comida
         * 
         * @return std::pair<int,int> 
         */
        std::pair<int,int> random_food();

        /**
         * @brief Inicia nova rodada
         * 
         */
        void new_update_round();

        /**
         * @brief Random das instancias escolhidas das coordenadas livres do pacman
         * 
         * @param alpha 
         * @param value_instance 
         * @return int 
         */
        int rand(int alpha, int value_instance);

        /**
         * @brief Introduction to new level
         * 
         */
        void new_level();

        /**
         * @brief Reiniciando o level
         * 
         */
        void restart_level();

        /**
         * @brief Processamento do game over para ser usado na View
         * 
         */
        void process_game_over();

};

#endif //SnakeGame_hpp