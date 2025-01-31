# Contador em matrix RGB por interrupção
    


Esse programa se refere a uma proposta de atividade do curso de capacitação para a residência EmbarcaTech. Alguns dos seus propósitos está em avaliar e demonstrar na prática o conhecimento e as possibilidade de aplicação da tecnologia de **Interrupções** disponibilizada no Pico SDK, na placa de desenvolvimento Pico W e por origem, no microcontrolador RP20240, utilizando-se para tal a plataforma educacional de desenvolvimento conhecida como BitDogLab.<br><br>


## O programa

Conforme solicitado, foi desenvolvido uma estrutura em código capaz de exibir em uma matriz de LEDs RGB e layout 5 x 5 uma representação numérica delimitada entre 0 e 9, onde dois botões, aqui chamados de **BUTTON_A** - à esquerda da placa - e **BUTTON_B** - à direita - são utilizados como os dispositivos de entrada responsáveis pelo incremento e decremento ordenado dos números a serem exibidos na matriz, por meio de interrupção.

Em seu início o programa executa a primeira exibição partindo do número '0' utilizando-se da função **show_number** que, entre outros parâmetros, deve ter presente o frame (número) que se deseja exibir. 

A efeito de intensidade dos LEDs está relacionado aos valores previamente configurados em cada um dos frames, utilzando-se para isso a função **apply_intensity_frame**. Ela recebe como um dos seus parâmetros o frame que se deseja definir as intensidades. 

Nesse ponto, por meio da função **apply_color_frame**, o número representado na matriz terá assumido a cor de index 0, de um total delimitado até 14, correspondente ao primeiro item do vetor de cores **color**. 
 
A razão pela qual foi criado um vetor de cores foi, por opção, utilizar o joystick presente na BitDogLAb, e atribuir ao seu botão de seleção a responsabilidade de alterar a cor dos LEDs utilizados pelos frames. 
Definido como **BUTTON_C** pelo programa, ele assume a habilidade de coletar o valor RGB contido no vetor de cores, de forma incremental ordenada, e aplicar aos LEDs, também por meio de interrupção.

Enquanto isso, no ciclo **normal** do loop infinito do código, o vermelho do LED RGB da placa pisca 5 vezes por segundo.

<br><br>
 
## Resumo das funcionalidades

- Redefinir a representação dos bits atribuindo valores de intensidades entre 0.0 e 1.0 e consequentemente aos LEDs de cada frame (número).
- Aplicar uma cor aos frames marcados com intensidades diferentes de 0.0, e por consequência, aos LEDs correspondentes na matriz.
- Incrementar e decrementar de forma ordenada o número exibido na matriz por interrupção.
- Alternar entre 15 cores possíveis e aplicá-las aos LEDs de cada frame por interrupção.


## Resultado
O vídeo do programa em execução pode ser conferido no endereço [https://youtube.com](https://youtube.com)
 