#ifndef Pacman_hpp
#define Pacman_hpp

#include "Bibliotecas.hpp"

class Pacman{
    private:
        std::pair<int, int> m_pacman_positions; //!Coordenadas (x,y) da localização do pacman no game

        int m_pacman_life; //!<Quantidade de vidas do pacman

        int m_food_pacman=0; //!<Quantidade de food recolhidas pelo pacman

    public:
        /**
        * @brief Construtor da classe Pacman
        */
        Pacman(std::pair<int,int> position_pacman, int pacman_life);
        /**
        * @brief Método que mostra mensagem positiva sobre a criação do pacman para o usuário
        */ 
        void criado_com_sucesso();
        /**
         * @brief Getter da posição do pacman
         * 
         * @return std::pair<int,int> 
         */
        std::pair<int,int> get_posicao_pacman();
        /**
         * @brief Setter da posição atual do pacman
         * @param x, posição x no plano cartesiano onde o pacman está
         * @param y, posição y no plano cartesiano onde o pacman está
         */
        void atualiza_posicao_pacman(int x, int y);

        /**
         * @brief Mostrar quantidade de comidas que o pacman recolheu
         * 
         * @return int 
         */
        int get_food_pacman();
        /**
         * @brief Método que atualiza o lado da cabeça do pacman!
         * @param string, recebe uma string 'UP','DOWN','LEFT','RIGHT' que indica o
         * movimento que ele vai fazer
         * @return char, o sentido da cabeça do pacman baseado no movimento que ele está
         * fazendo 
         * Esta função é chamada em tempo real na classe View().
         */
        char movement_pacman_side(std::string);

        /**
         * @brief Coletando comidinha
         * 
         * @param food 
         */
        void set_food_pacman();

        /**
         * @brief Retorna quantidade de vidas do pacman
         * 
         * @return int 
         */
        int get_pacman_live();

        /**
         * @brief Método que tira uma vida do pacman
         * 
         */
        void loss_of_life();

};

#endif