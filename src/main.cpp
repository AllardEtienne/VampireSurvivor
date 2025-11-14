#include <SFML/Graphics.hpp>

void center_horizontally(const auto& parent, auto& child)
{
    const auto parentBoundingBox = parent.getLocalBounds();
    const sf::Vector2f parentLocalCenter{parentBoundingBox.left + parentBoundingBox.width * 0.5f,
                                         parentBoundingBox.top + parentBoundingBox.height * 0.5f};

    const sf::Vector2f parentGlobalCenter = parent.getTransform().transformPoint(parentLocalCenter);

    const auto childBoundingBox = child.getLocalBounds();
    const auto childOrigin = child.getOrigin();
    child.setOrigin(childBoundingBox.left + childBoundingBox.width * 0.5f, childOrigin.y);

    const auto childPosition = child.getPosition();
    child.setPosition(parentGlobalCenter.x, childPosition.y);
}

void center_vertically(const auto& parent, auto& child)
{
    const auto parentBoundingBox = parent.getLocalBounds();
    const sf::Vector2f parentLocalCenter{parentBoundingBox.left + parentBoundingBox.width * 0.5f,
                                         parentBoundingBox.top + parentBoundingBox.height * 0.5f};

    const sf::Vector2f parentGlobalCenter = parent.getTransform().transformPoint(parentLocalCenter);

    const auto childBoundingBox = child.getLocalBounds();
    const auto childOrigin = child.getOrigin();
    child.setOrigin(childOrigin.x, childBoundingBox.top + childBoundingBox.height * 0.5f);

    const auto childPosition = child.getPosition();
    child.setPosition(childPosition.x, parentGlobalCenter.y);
}
int main()
{
    // Variable pour savoir quel bouton est sélectionné
    // true pour "Start", false pour "Quit"
    bool button = true;

    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf"))
    {
        return -1; // erreur lors du chargement de la police
    }

    auto colorStart = sf::Color::White;
    auto colorQuit = sf::Color::White;
    // Largeur et hauteur de la fenêtre
    int widthWindow = 800, heightWindow = 600;

    // Création d'une fenêtre "render"
    sf::RenderWindow window(sf::VideoMode(widthWindow, heightWindow), "VampireSurvivor",
                            sf::Style::Default); // Pour un style personnalisé
    // Boucle principale avec gestion frame par frame
    while (window.isOpen())
    {
        // Gestion des évènements
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Demande de fermeture (croix de la fenêtre, ALT+F4)
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return 0;
            }


            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                    return 0;
                }
                else if (event.key.code == sf::Keyboard::Up)
                {
                    // true pour "Start", false pour "Quit"
                    button = true;
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    // true pour "Start", false pour "Quit"
                    button = false;
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    // true pour "Start", false pour "Quit"
                    if (button)
                    {
                        
                    }
                    else
                    {
                        // Quitter le jeu
                        window.close();
                        return 0;
                    }
                }
            }


            // D'autres évènements peuvent être gérés ici
        }
        


        if (button)
        {
            colorStart = sf::Color::Green;
            colorQuit = sf::Color::White;
        }
        else
        {
            colorStart = sf::Color::White;
            colorQuit = sf::Color::Green;
        }

        // Effacement de l'ancienne frame (framebuffer)
        window.setVerticalSyncEnabled(true);

        window.clear(sf::Color::Black);


        // ... Ici on dessine les objets, sprites, textes, formes
        sf::VertexArray triangleStart(sf::Triangles, 3);
        // Définition de la position et de la couleur de chaque sommet
        triangleStart[0].position = sf::Vector2f(400.f, 30.f);
        triangleStart[0].color = colorStart;
        triangleStart[1].position = sf::Vector2f(300.f, 150.f);
        triangleStart[1].color = colorStart;
        triangleStart[2].position = sf::Vector2f(500.f, 150.f);
        triangleStart[2].color = colorStart;
        // Dans la boucle de rendu
        window.draw(triangleStart);


        sf::VertexArray triangleQuit(sf::Triangles, 3);
        // Définition de la position et de la couleur de chaque sommet
        triangleQuit[0].position = sf::Vector2f(400.f, 570.f);
        triangleQuit[0].color = colorQuit;
        triangleQuit[1].position = sf::Vector2f(300.f, 450.f);
        triangleQuit[1].color = colorQuit;
        triangleQuit[2].position = sf::Vector2f(500.f, 450.f);
        triangleQuit[2].color = colorQuit;
        // Dans la boucle de rendu
        window.draw(triangleQuit);


        sf::Text textStart;
        textStart.setFont(font);
        textStart.setString("Start");
        textStart.setCharacterSize(50);               // Taille en pixels
        textStart.setFillColor(colorStart); // Couleur du texte
        textStart.setPosition(300.f, 200.f);          // Position sur l'écran

        window.draw(textStart);

        sf::Text quitText;
        quitText.setFont(font);
        quitText.setString("Quit");
        quitText.setCharacterSize(50);                // Taille en pixels
        quitText.setFillColor(colorQuit); // Couleur du texte
        quitText.setPosition(300.f, 350.f);           // Position sur l'écran

        

        window.draw(quitText);
        // Affiche la nouvelle frame à l'écran
        window.display();
    }
    return 0;
}

