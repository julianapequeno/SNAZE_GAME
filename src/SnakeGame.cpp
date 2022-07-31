#include "../include/SnakeGame.hpp"
#include "../include/Player.hpp"

#include <iostream>
#include <fstream>


#include <chrono> //por causa do sleep
#include <thread> //por causa do sleep

#include <algorithm>
#include <iterator>
#include <string>
#include <cstring>

using namespace std;

/**
 * @brief Método que retorna menssagem de alerta
 * 
 * @param key -- warn, error, sucess
 * @param message -- qualquer mensagem
 */
void alert(std::string key ,std::string message){
    if(key == "warn"){
        std::cout<<ANSI_COLOR_YELLOW<<message<<ANSI_COLOR_RESET<<std::endl;
    }
    if(key == "error"){
        std::cout<<ANSI_COLOR_RED<<message<<ANSI_COLOR_RESET<<std::endl;
    }
    if(key == "sucess"){
        std::cout<<ANSI_COLOR_GREEN<<message<<ANSI_COLOR_RESET<<std::endl;
    }
}

/**
 * @brief função auxiliar para limpar o terminal
 */
void clearScreen(){
//some C++ voodoo here ;D
#if defined _WIN32
    system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#elif defined (__APPLE__)
    system("clear");
#endif
}

SnakeGame::SnakeGame(string levels, string mode, string ia, bool debug){
    m_levels_file=levels;
    m_choice = "";
    m_frameCount = 0;
    m_current_level = 0;
    m_score = 0;
    m_step_counter = 0;

    m_level = std::vector<Level>(3); //Teste com 3 levels

    if(debug && mode.compare("pacmaze")==0)
        PACMAZE_DEBUG = true;
    else if(debug && mode.compare("snaze")==0)
        std::cout << "";
       // SNAZE_DEBUG = true;
    else if(mode.compare("pacmaze")==0)
        PACMAZE = true;
    else if(mode.compare("snaze")==0)
        SNAZE = true;
    
    if(ia.compare("random")==0)
        MODE = false;
    else if(ia.compare("find")==0)
        MODE = true;
    
    initialize_game();

}

/**
 * @brief função auxiliar para fazer o programa esperar por alguns milisegundos
 * @param ms a quantidade de segundos que o programa deve esperar
 */
void wait(int ms){
    this_thread::sleep_for(chrono::milliseconds(ms));
}

/*----------------- INICIALIZAÇÃO DO GAME -------------------*/
void SnakeGame::initialize_game(){
    ifstream levelFile(m_levels_file); 
    int lineCount = 0;
    string line;

    std::locale loc;
    if(levelFile.is_open()){    
        while(getline(levelFile, line)){ 
            if(lineCount == 0){ //são os dados de tamanho da matriz
                //linhas - colunas - levels
                stringstream ss;
                ss<<line;
                int num_value_map;
                for(int i = 0; i < 3; i++){
                    ss>>num_value_map;
                    this->values_map.push_back(num_value_map);
                }
            } 
            lineCount++;
        }
    }
    //values_map[0] - linhas, values_map[1] - colunas, values_map[3] - life
    
    //---------------
    m_level[m_current_level] = Level(m_levels_file, values_map[0], values_map[1], values_map[2]);
    m_maze_assembly = m_level[m_current_level].get_process_map();
    wait(2000);
    set_header();
    bool encontrou_pacman = m_level[m_current_level].get_status_pacman();
    if(encontrou_pacman == true){
        this->m_pacman = new Pacman(m_level[m_current_level].get_pacman_position_found(), 5);
    }


    if(!encontrou_pacman){
        alert("error", "Não foi possível encontrar o pacman, digite exit para sair");
        m_state = WAITING_USER;
    }else{
        /*Iniciando ia*/
        m_state = WAITING_USER; //estado inicial é WAITING_USER, mas poderia ser outro
        m_ia_player = Player();
        if(SnakeGame::PACMAZE || SnakeGame::PACMAZE_DEBUG)
            m_ia_player.initialize_pacman(m_pacman);
        //else if(SnakeGame::SNAZE)
            /*Função de inicializar a snake*/

        /*iniciando comida*/
        m_posicao_food.first  = random_food().first;
        m_posicao_food.second = random_food().second; 

        /*iniciando mapas na IA*/
        m_ia_player.recebe_mapa_assembly(m_maze_assembly);
        m_view.initialize_maze_assembly_debug(m_maze_assembly.size(),m_maze_assembly.at(0).size());

        this->m_ia_player.atualiza_posicao_food(this->m_posicao_food.first,this->m_posicao_food.second);
        
        /*iniciando modo do jogo + busca pelo caminho*/
        if(SnakeGame::MODE){
            bool solution = this->m_ia_player.find_solution(m_pacman->get_posicao_pacman().first,m_pacman->get_posicao_pacman().second);
            if(!solution){
                std::cout << "Não há solução :(" << std::endl;
                /*Solução para que ele vá numa direção aleatória e morra: chama o find_solution__aleatory*/
                 m_ia_player.m_call_find_aleatory++;
                this->m_ia_player.find_solution__aleatory(m_pacman->get_posicao_pacman().first,m_pacman->get_posicao_pacman().second);
                SnakeGame::MODE = false; //automaticamente muda para o modo aleatório
                wait(2000);
            }
        }
        std::cout<< BG_MAGENTA ANSI_COLOR_GRAY "               B E M   V I N D O   A O   P A C M A Z E" BG_RESET ANSI_COLOR_RESET<<std::endl;
        m_view.update_info_header(m_score, values_map[2], m_pacman->get_food_pacman(), m_pacman->get_pacman_live());
        m_view.view_header(m_header);

        if(SnakeGame::PACMAZE_DEBUG && SnakeGame::MODE)
            this->m_ia_player.find_solution_debug(m_maze_assembly,m_posicao_food.first,m_posicao_food.second);
        else
            m_view.view_maze(m_maze_assembly, this->m_pacman->movement_pacman_side(this->m_ia_player.m_move_view));
        std::cout<<"\nQuantidade de comidas: "<< m_pacman->get_pacman_live() <<std::endl;

    }
}

/*--------------PROCESSOS DAS AÇÕES--------------*/
void SnakeGame::process_actions(){
    switch(m_state){
        case WAITING_USER: //o jogo bloqueia aqui esperando o usuário digitar a escolha dele
            std::cout<<"-> ";
            std::cin>>m_choice;
            break;
        case WAITING_IA:
            if(MODE){
                m_ia_player.set_last_move(m_action);
                m_action = m_ia_player.next_move();
            }else{
                m_ia_player.m_call_find_aleatory++;
                if(m_ia_player.m_call_find_aleatory == 700) {//!Limite de chamadas do aleatory, até o pac morrer
                    m_ia_player.m_error_factor = true;
                }
                m_ia_player.find_solution__aleatory(this->m_ia_player.get_posicao_jogador().first, this->m_ia_player.get_posicao_jogador().second);
                m_action = m_ia_player.next_aleatory_move(this->m_ia_player.get_posicao_jogador().first, this->m_ia_player.get_posicao_jogador().second);
                
            }
            break;
        default:
            break;
    }
}

/*-----------------UPDATE----------------*/
void SnakeGame::update(){
    switch(m_state){
        case RUNNING:
            if(status_level_game == false){
                m_state = WAITING_USER;
            } else {
                m_state = WAITING_IA;
            }
            //if(m_frameCount>0 && m_frameCount%10 == 0) //depois de 10 frames o jogo pergunta se o usuário quer continuar
                //m_state = WAITING_USER;   //SO PRA FAZER TESTES DEIXEI COMENTADO
            //else
            break;
        case WAITING_USER: //se o jogo estava esperando pelo usuário então ele testa qual a escolha que foi feita
            if(m_choice == "n"){
                m_state = GAME_OVER;
            }
            else{
                if(status_level_game == false){
                    if(m_current_level == 2){
                        //!Aqui um fim de jogo!
                        std::cout << ANSI_COLOR_GREEN "Parabéns! Zerou o jogo!" ANSI_COLOR_RESET<< std::endl;
                        m_state = GAME_OVER;
                    }else{
                        new_level();
                        wait(1000);
                    }
                } else {
                    m_state = RUNNING;
                }
            }
            break;
        case WAITING_IA: 
            if(m_action == 1){//UP
                this->m_ia_player.set_posicao_jogador(this->m_ia_player.get_posicao_jogador().first-1,this->m_ia_player.get_posicao_jogador().second);
                /*Lembre-se de atualizar a posição do pacman no Player.cpp --> ATUALIZA NO FIND SOLUTION*/
                this->m_ia_player.m_move_view = "UP";
                m_state = RUNNING;
            }else if(m_action == 2){//DOWN
                this->m_ia_player.set_posicao_jogador(this->m_ia_player.get_posicao_jogador().first+1,this->m_ia_player.get_posicao_jogador().second);
                this->m_ia_player.m_move_view = "DOWN";
                m_state = RUNNING;
            }else if(m_action == 3){//LEFT
                this->m_ia_player.set_posicao_jogador(this->m_ia_player.get_posicao_jogador().first,this->m_ia_player.get_posicao_jogador().second-1);
                this->m_ia_player.m_move_view = "LEFT";
                m_state = RUNNING;
            }else if(m_action == 4){//RIGHT
                this->m_ia_player.set_posicao_jogador(this->m_ia_player.get_posicao_jogador().first,this->m_ia_player.get_posicao_jogador().second+1);
                this->m_ia_player.m_move_view = "RIGHT";
                m_state = RUNNING;
            }else if(m_action == 0){ //STAY
                m_state = RUNNING;
                if(SnakeGame::MODE){
                    this->m_ia_player.set_posicao_jogador(this->m_ia_player.get_posicao_jogador().first,this->m_ia_player.get_posicao_jogador().second);
                    m_score = m_score + m_level[m_current_level].increment_score(m_step_counter);
                    bool ended = m_level[m_current_level].end_of_level(m_pacman->get_food_pacman());

                    if(!ended){ //ainda tem comida
                        std::cout<< BG_MAGENTA ANSI_COLOR_BLACK "P R Ó X I M O   R O U N D" BG_RESET ANSI_COLOR_RESET <<std::endl;
                        m_step_counter = 0;
                        m_pacman->set_food_pacman();
                        new_update_round();
                    } else {
                        status_level_game = false;
                    }
                }else{
                    m_state = GAME_OVER;
                }
                wait(1000);
            }
            
            if(SnakeGame::PACMAZE_DEBUG){
                if(this->m_ia_player.m_call_find_aleatory >=700){ //!Testa os limites para conferir se o pacman não morreu
                    auto try_this_position = m_ia_player.get_posicao_jogador();
                    /*Testa os limites caso o modo random esteja rodando*/
                    if(this->m_ia_player.get_posicao_jogador().first >= values_map[0])
                        this->m_ia_player.set_posicao_jogador(values_map[0]-1,this->m_ia_player.get_posicao_jogador().second);
                    else if(this->m_ia_player.get_posicao_jogador().first < 0)
                        this->m_ia_player.set_posicao_jogador(1,this->m_ia_player.get_posicao_jogador().second);
                    
                    if(this->m_ia_player.get_posicao_jogador().second >= values_map[1])
                        this->m_ia_player.set_posicao_jogador(m_ia_player.get_posicao_jogador().first,values_map[1]-1);
                    else if(this->m_ia_player.get_posicao_jogador().second < 0)
                        this->m_ia_player.set_posicao_jogador(m_ia_player.get_posicao_jogador().first,1);

                    m_maze_assembly = m_level[m_current_level].update_assembler_maze(try_this_position, this->m_posicao_food,true,m_action);
                }else{
                    m_maze_assembly = m_level[m_current_level].update_assembler_maze(this->m_ia_player.get_posicao_jogador(), this->m_posicao_food,true,m_action);
                }
            }else{
                 m_maze_assembly = m_level[m_current_level].update_assembler_maze(this->m_ia_player.get_posicao_jogador(), this->m_posicao_food,false,m_action);
            }
            m_step_counter++;

            //wait(2000);
            break;
        default:
            
            break;
    }
}

/*--------------------RENDERIZAÇÃO----------------*/
void SnakeGame::render(){
    switch(m_state){
        case RUNNING:
            clearScreen();
            std::cout<<"\n";
            m_view.update_info_header(m_score, values_map[2], m_pacman->get_food_pacman(), m_pacman->get_pacman_live());
            m_view.view_header(m_header);
            if(SnakeGame::PACMAZE_DEBUG)
                m_view.view_maze_debug(m_maze_assembly,this->m_pacman->movement_pacman_side(this->m_ia_player.m_move_view),this->m_ia_player.get_last_move(),this->m_pacman->get_posicao_pacman());
            else
                m_view.view_maze(m_maze_assembly,this->m_pacman->movement_pacman_side(this->m_ia_player.m_move_view));
            cout<<"fc:"<<m_frameCount<<endl;
            cout<< BG_MAGENTA ANSI_COLOR_BLACK "Level: " << m_current_level + 1<< ANSI_COLOR_RESET BG_RESET <<std::endl;

            
            if(!m_level[m_current_level].status_player_maze()){
                alert("error", "BATEU NA BARREIRA!");
                m_pacman->loss_of_life();
                std::cout<<"Vidas atuais: "<<m_pacman->get_pacman_live()<<std::endl;
                wait(4000);
                if(m_pacman->get_pacman_live() == 0){
                    m_state = GAME_OVER;
                    game_over();
                } else {
                    restart_level();
                }
            }
            break;
        case WAITING_USER:
            cout<<"Você quer iniciar/continuar o jogo? (s/n)"<<endl;
            break;
        case GAME_OVER:
            game_over();
            break;
    }
    m_frameCount++;
}

/*---------------GAME OVER------------------*/
void SnakeGame::game_over(){
    process_game_over();
    wait(2000);
    this->m_ia_player.despedindo_pacman(); //!Adicionei isso aqui para que ele desaloque o pacman da memória

}

/*---------------LOOP---------------------*/
void SnakeGame::loop(){
    render(); //chama um render para a interface inicial
    while(m_state != GAME_OVER){
        process_actions();
        update();
        render();
        wait(70);// espera 1 segundo entre cada frame
    }
}

/*----------CABEÇALHO DO GAME--------------*/
void SnakeGame::set_header(){
    this->m_header = std::vector< std::vector<char> > (8, std::vector<char>(values_map[1]));
    
    std::string dado_line;
    ifstream headerFile("../data/header.txt"); 
    if(headerFile.is_open()){
        alert("sucess", ">> Arquivo encontrado");
        while(getline(headerFile, dado_line)){
            this->m_maze_header.push_back(dado_line);
        }
    } else {
        alert("error", ">> Arquivo não encontrado");
    }

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 73; j++){
            this->m_header.at(i).at(j) = m_maze_header.at(i).at(j);
        }
    }
}


/*--------- SORTEIO DA COMIDA NO LABIRINTO -----------*/

std::pair<int,int> SnakeGame::random_food(){ 
    int rand_inst = std::rand()%m_level[m_current_level].get_coords_free().size();
    return m_level[m_current_level].get_coords_free()[rand_inst];
}

void SnakeGame::new_update_round(){
    /*Atualiza a posição da comida de acordo com o ANTIGO MAZE para ver pos que nunca foi*/
    std::pair<int,int> instance_coords = random_food();
    m_posicao_food.first  = instance_coords.first;
    m_posicao_food.second = instance_coords.second;
    this->m_ia_player.atualiza_posicao_food(this->m_posicao_food.first,this->m_posicao_food.second);
    /*Reinicializando o mapa em assembly*/
    m_maze_assembly = m_level[m_current_level].get_process_map();
    m_ia_player.recebe_mapa_assembly(m_maze_assembly);
    m_view.initialize_maze_assembly_debug(m_maze_assembly.size(),m_maze_assembly.at(0).size());

    if(SnakeGame::MODE){
        /*Resetando a função find, para encontrar o próximo caminho*/
        m_ia_player.resetar_find();
        /*Chamando find_solution()*/
        bool solution = this->m_ia_player.find_solution(m_pacman->get_posicao_pacman().first,m_pacman->get_posicao_pacman().second);
        if(!solution){ 
            std::cout << "Não há solução :(" << std::endl;
            m_ia_player.m_call_find_aleatory++;
            if(m_ia_player.m_call_find_aleatory == 700) {//!Limite de chamadas do aleatory, até o pac morrer
                m_ia_player.m_error_factor = true;
            }
            this->m_ia_player.find_solution__aleatory(m_pacman->get_posicao_pacman().first,m_pacman->get_posicao_pacman().second);
            SnakeGame::MODE = false; //automaticamente muda para o modo aleatório
            wait(2000);
        }else{
            if(SnakeGame::PACMAZE_DEBUG){
                clearScreen();
                m_view.update_info_header(m_score, values_map[2], m_pacman->get_food_pacman(), m_pacman->get_pacman_live());
                m_view.view_header(m_header);

                if(SnakeGame::PACMAZE_DEBUG && SnakeGame::MODE)
                    this->m_ia_player.find_solution_debug(m_maze_assembly,m_posicao_food.first,m_posicao_food.second);
            }else{
                alert("sucess", "Tem solução!");
            }
        }
    }
    wait(2000);
}

void SnakeGame::new_level(){
    status_level_game = true;
    m_current_level = m_current_level + 1; 
    std::cout<<BG_MAGENTA ANSI_COLOR_BLACK " --------[ P R Ó X I M O   L E V E L ]--------" BG_RESET ANSI_COLOR_RESET<<std::endl;

    Level novo_level =  Level(m_levels_file, values_map[0], values_map[1], values_map[2]);
    m_level[m_current_level] = novo_level;

    this->m_maze_assembly = m_level[m_current_level].get_process_map();
    set_header();

    std::pair<int,int> instance_coords = random_food();
    m_posicao_food.first  = instance_coords.first;
    m_posicao_food.second = instance_coords.second;
    this->m_ia_player.atualiza_posicao_food(this->m_posicao_food.first,this->m_posicao_food.second);
    m_level[m_current_level].set_pacman_position_found_new_level(m_pacman->get_posicao_pacman());

    this->m_ia_player.mudando_mapa();
    this->m_ia_player.recebe_mapa_assembly(m_maze_assembly);//!Recebe novo mapa assembly
    
    delete m_pacman; //!Resetando o pacman
    this->m_pacman = new Pacman(m_level[m_current_level].get_pacman_position_found(), 5);
    m_ia_player.initialize_pacman(m_pacman);

    m_ia_player.resetar_find();

    bool solution = this->m_ia_player.find_solution(m_pacman->get_posicao_pacman().first,m_pacman->get_posicao_pacman().second);
    if(!solution){
        alert("error", "Não há solução :(");
         m_ia_player.m_call_find_aleatory++;
        if(m_ia_player.m_call_find_aleatory == 700) {//!Limite de chamadas do aleatory, até o pac morrer
            m_ia_player.m_error_factor = true;
        }
        this->m_ia_player.find_solution__aleatory(m_pacman->get_posicao_pacman().first,m_pacman->get_posicao_pacman().second);
        SnakeGame::MODE = false; //automaticamente muda para o modo aleatório

    } else {
        alert("sucess", "Tem solução!");
    }
}

void SnakeGame::restart_level(){
    alert("warn", "Reiniciando level");
    this->m_ia_player.set_posicao_jogador(m_level[m_current_level].get_pacman_position_found().first, m_level[m_current_level].get_pacman_position_found().second);
    
    m_level[m_current_level].reset_food_dispo();
    m_score = 0;
    wait(1000);
}

void SnakeGame::process_game_over(){
    ifstream gameOver("../data/game_over.txt");
    if(gameOver.is_open()){
        std::string over;
        while(getline(gameOver, over)){
            m_game_over.push_back(over);
        }

        m_view.view_game_over(m_game_over);
        wait(4000);
    } else {
        alert("error", "Arquivo Game Over não encontrado");
        wait(7000);
    }
}
