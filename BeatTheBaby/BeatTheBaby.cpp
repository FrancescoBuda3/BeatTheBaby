#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>

int main() {
    // Carica il file audio
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("test.wav")) {
        std::cerr << "Errore nel caricare il file audio" << std::endl;
        return -1;
    }

    // Crea un oggetto suono e lo associa al buffer caricato
    sf::Sound sound(buffer);

    // Carica il file audio
    sf::SoundBuffer buffer2;
    if (!buffer2.loadFromFile("test2.wav")) {
        std::cerr << "Errore nel caricare il file audio" << std::endl;
        return -1;
    }

    // Crea un oggetto suono e lo associa al buffer caricato
    sf::Sound sound2(buffer2);

    // Crea un oggetto clock per tenere traccia del tempo
    sf::Clock clock;

    // Imposta l'intervallo di tempo a 5 secondi
    sf::Time intervallo = sf::seconds(5.f);

    // Loop principale del gioco
    while (true) {
        // Verifica se è passato l'intervallo di tempo
        if (clock.getElapsedTime() >= intervallo) {
            // Riproduci il suono
            sound.play();
            sound2.play();

            // Azzera il timer
            clock.restart();
        }

        

        // Aggiorna altre parti del gioco qui...

        // Assicura che il loop non esegua troppo velocemente
        sf::sleep(sf::milliseconds(10));
    }

    return 0;
}