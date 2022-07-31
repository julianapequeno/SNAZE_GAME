#ifndef Player_hpp
#define Player_hpp

#include "Pacman.hpp"
#include "Bibliotecas.hpp"
#include <cstdlib>
#include <ctime>

class Player{
    private:
        Pacman *m_pacman; //! Objeto da classe pacman. Nosso jogador!
        std::vector<std::vector<int>> m_maze_assembly; //! Vector do labirinto atual em 0 e 1
    public:
        enum VIZINHOS{
            UP, //<! Pode se mover para cima
            DOWN, //<! pode se mover para baixo
            LEFT, //<! pode se mover para o lado esquerdo
            RIGHT, //<! pode se mover para o lado direito
            STAY //<! SnakeGame::MODE::True - Ele se mantém no mesmo lugar | SnakeGame::MODE::False - O player morreu
        };

    private:
        std::vector<std::tuple<VIZINHOS,bool,std::pair<int,int>>> m_moves; //! Possibilidades de movimentos que o pacman/snaze pode fazer sem morrer
        std::vector<VIZINHOS> m_player_moves; //! Movimentos até a comida - Vector com as escolhas do pacman/snaze
        std::pair<int,int> m_localiza_comida; //! Localização atual da comida no jogo
        std::vector<std::vector<bool>> m_rastro = std::vector< std::vector<bool> > (19, std::vector<bool>(73)); //!Por onde o pacman já passou
        std::vector<int> m_way; //!Vector que guarda o caminho que o pacman/snaze vai percorrer no modo find
        int last_move; //!Guarda o último movimento do pacman/snaze
    public:
        /**
         * @brief Fator ativado quando o pacman atinge uma quantidade x de vezes 
         * em que chamou o find_solution__aleatory
         */
        bool m_error_factor = false;
        /**
         * @brief Quantidade de vezes que a função find aleatória foi chamada
         */
        int m_call_find_aleatory = 0; 
        /**
         * @brief Atributo que guarda o movimento do pacman/snaze em string
         * Inicialmente está virado para a direita
         */
        std::string m_move_view = "RIGHT";
        /**
         * @brief Atributo que indica que o programa encontrou a posição da comida
         */
        bool FOUND_A_WAY = false; 
        /**
         * @brief Atributo que indica ao programa que é a snaze que está jogando.
         * Ajuda a ativar os métodos feitos para esse jogador
         */
        bool SNAZE_PLAYING = false;
        /**
         * @brief Atributo qeu indica ao programa que é o pacman que está jogando.
         * Ajuda a ativar os métodos feitos para esse jogador.
         */
        bool PACMAN_PLAYING = true;
        /**
         * @brief Contrutor da classe Player
         */
        Player();
        /**
         * @brief Método que recebe endereço de um objeto pacman, e guarda
         * o jogador na IA que rodará o jogo.
         * @param pacman, o endereço do pacman que jogará o jogo 
         */
        void initialize_pacman(Pacman *pacman);
        /**
         * @brief Método que desaloca o pacman da memória, já
         * que ele é uma variável dinâmica;
         */
        void despedindo_pacman();
        /**
         * @brief Método que deleta o mapa (desaloca) da memória.
         * Evita furos na memória deletando o m_maze_atual_player e o m_maze_assembly
         */
        void mudando_mapa();
        /**
         * @brief Método que recebe o mapa em 0´s e 1´s da classe SnakeGame.
         */
        void recebe_mapa_assembly(std::vector<std::vector<int>>);
        /**
         * @brief Método que mostra as possibilidades de movimentos que o pacman/snaze
         * pode escolher. Apenas para mostrar. -> usar depois para identificar cada caminho.
         */
        void my_journey();
        /**
         * @brief Método que encontra uma solução válida aleatória para o caminho do pacman/snaze
         * @param x, posição x do pacman/snaze
         * @param y, posição y do pacman/snaze
         * @return true, se encontrou e armazenou uma sequência (movimentos) de passos até a comida
         * Caso tenha encontrado, armazena no objeto m_pacman, o caminho até a comida
         * @return false, se não encontrou um resultado
         */
        void find_solution__aleatory(int x, int y);
        /**
         * @brief Método que retorna a próxima ação a ser executada pelo jogador.
         * @param x, coordenada x do jogador
         * @param y, coordenada y do jogador
         * @return int, um número que indica qual o próximo movimento do player
         */
        int next_aleatory_move(int x, int y);
        /**
         * @brief Retorna a posição atual do jogador.
         * @return std::pair<int,int>, um std::pair com (x,y) do jogador
         */
        std::pair<int,int> get_posicao_jogador();
        /**
         * @brief Recebe a nova posição do jogador de acordo com a sua escolha de 
         * próximo movimento.
         * @param x, próxima coordenada x do jogador.
         * @param y, próxima coordenada y do jogador.
         */
        void set_posicao_jogador(int x, int y);
        /**
         * @brief Método que encontra uma solução válida para o caminho do pacman/snaze
         * @param x, posição x do pacman/snaze
         * @param y, posição y do pacman/snaze
         * @return true, se encontrou e armazenou uma sequência (movimentos) de passos até a comida
         * Caso tenha encontrado, armazena no objeto m_pacman, o caminho até a comida
         * @return false, se não encontrou um resultado
         */
        bool find_solution(int x, int y);
        /**
         * Retorna a próxima ação do jogador um mapeamento de 0-3, representando ir para esquerda, cima, direita, baixo
         * @return a próxima ação do jogador
         **/
        int next_move();
        /**
         * @brief Método que retorna se o pacman/snaze
         * encontrou a comida.
         * @param x 
         * @param y 
         * @return true 
         * @return false 
         */
        bool encontrou(int x, int y);
        /**
         * @brief Método que atualiza a posição da comida
         * no tabuleiro.
         */
        void atualiza_posicao_food(int, int);
        /**
         * @brief Algoritmo que verifica se um lugar da matriz é
         * permitida para o player a ocupá-la
         * @return true, caso sim
         * @return false, caso não, ou seja, se for parede 
         */
        bool permitido(int, int);
        /**
         * @brief Algoritmo qeu verifica se um lugar da matriz já
         * foi visitado pelo player
         * @return true, caso já tenha sido visitada
         * @return false, caso não tenha sido visitada ainda
         */
        bool visitado(int, int);
        /**
         * @brief Set the last move object
         * 
         * @param move 
         */
        void set_last_move(int move);
        /**
         * @brief retorna o último movimento do player
         * 
         * @return int 
         */
        int get_last_move();
        
        /**
         * @brief Mostra a solução inicial do find_solution no modo debug. Ou seja, todo o caminho
         * do pacman
         * @param maze_map 
         */
        void find_solution_debug(std::vector<std::vector<int>> maze_map,int,int);

        /**
         * @brief Reinicia o cálculo do find_solution para o próximo round
         * 
         */
        void resetar_find();

        /**
         * @brief Mostra a solução encontrada pelo find_Solution.
         *  Método para visualização textual do caminho escolhido pelo
         * método find_solution()
         */
        void show_m_player_moves();



};
#endif //Player_hpp