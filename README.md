<h1 align="center">EmbarcaTech | Atividade Interrupções</h1>

Para consolidar a compreensão dos conceitos relacionados ao uso de interrupções no microcontrolador RP2040 e explorar as funcionalidades da placa de desenvolvimento BitDogLab, propõe-se a seguinte tarefa prática. 

## Objetivos:

• Compreender o funcionamento e a aplicação de interrupções em microcontroladores.
• Identificar e corrigir o fenômeno do bouncing em botões por meio de debouncing via software.
• Manipular e controlar LEDs comuns e LEDs endereçáveis WS2812.
• Fixar o estudo do uso de resistores de pull-up internos em botões de acionamento.
• Desenvolver um projeto funcional que combine hardware e software.

## Materiais utilizados:

Neste projeto, são utilizados os seguintes componentes conectados à placa BitDogLab:
1) Matriz 5x5 de LEDs (endereçáveis) WS2812, conectada à GPIO 7.
2) LED RGB, com os pinos conectados às GPIOs (11, 12 e 13).
3) Botão A conectado à GPIO 5.
4) Botão B conectado à GPIO 6.

## Lista de requisitos:

- Uso de interrupções: Todas as funcionalidades relacionadas aos botões devem ser implementadas
utilizando rotinas de interrupção (IRQ).
- É obrigatório implementar o tratamento do bouncing dos botões via software.
- O projeto deve incluir o uso de LEDs comuns e LEDs WS2812, demonstrando o domínio de diferentes tipos de controle. 
- O código deve estar bem estruturado e comentado para facilitar o
entendimento.

## Tecnologias:

1. Git e Github;
2. VScode;
3. Linguagem C;
4. Simulador Wokwi;
5. Placa de desenvolvimento BitDogLab

## Funcionalidades Implementadas:
1. O LED vermelho do LED RGB pisca continuamente 5 vezes por segundo.
2. O botão A incrementa o número exibido na matriz de LEDs cada vez que for pressionado.
3. O botão B decrementa o número exibido na matriz de LEDs cada vez que for pressionado.
4. Os LEDs WS2812 criam efeitos visuais representando números de 0 a 9.

Essa lógica permite controlar a exibição na matriz de LEDs de forma precisa, sem efeitos indesejados causados pelo bouncing dos botões.

## 💻 Instruções para Importar, Compilar e Rodar o Código Localmente:

Siga os passos abaixo para clonar o repositório, importar no VS Code usando a extensão do **Raspberry Pi Pico Project**, compilar e executar o código.

1. **Clone o repositório para sua máquina local**  
   Abra o terminal e execute os comandos abaixo:
   ```bash
   git clone https://github.com/usuario/projeto.git
   cd projeto

2. **Abra o VS Code e instale a extensão "Raspberry Pi Pico Project" (caso não já a tenha instalada)**
 - No VS Code, vá até "Extensões" (Ctrl+Shift+X)
 - Pesquise por "Raspberry Pi Pico Project"
 - Instale a extensão oficial

3. **Importe o projeto no VS Code**
 - No VS Code, na barra lateral do lado esquerdo clique em "Raspberry Pi Pico Project" <img src="images/icon_raspberry_pico_project.png" width="25px">
 - No menu que aparecer clique em <img src="images/icon_import_project.png" height="25px">
 - Clicando em "Change" escolha a pasta clonada do repositório
 - Escolha a versão do SDK 2.1.0
 - Clique em "Import"
 - Abra a pasta atividade 1 para compilação



## Desenvolvedor:
 
<table>
  <tr>
    <td align="center"> <sub><b> Sara Souza Ferreira </b></td>
    </tr>
</table>

## Link do video de apresentação: 

<table>
  <tr>
    <td align="center"> <sub><b> https://youtube.com/shorts/hKGRBVfThpQ?si=iXkc68QORvWiUUg0 </b></td>
    </tr>
</table>
