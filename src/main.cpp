#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <fstream>

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

class CShape 
{
    public:
        bool draw = true;
        std::string name;
        sf::Vector2f position;
        sf::Vector2f speed;
        sf::Color color;
        float scale = 1.f;

        CShape() {}
        CShape(
            std::string name, 
            sf::Vector2f pos, 
            sf::Vector2f speed,
            sf::Color color
        ) : name(name), position(pos), speed(speed), color(color) {}
        virtual ~CShape() {}

        virtual std::string to_string() const {
            std::ostringstream ss;
            ss << name << ", draw=" << draw 
                << ", scale=" << scale << ", "
                << "position (" << position.x << "," << position.y << "), "
                << "speed (" << speed.x << "," << speed.y << "), "
                << "color (" 
                << int(color.r) << "," 
                << int(color.g) << ","
                << int(color.b) << ") ";

            return ss.str();
        }
};

class CRectangle : public CShape 
{
    public:
        sf::Vector2f size;
        CRectangle() {};
        CRectangle(
            std::string name, 
            sf::Vector2f pos, 
            sf::Vector2f speed,
            sf::Color color,
            sf::Vector2f size
        ): CShape(name, pos, speed, color), size(size) {}

        std::string to_string() const {
            std::ostringstream ss;
            ss  << CShape::to_string()
                << ", size (" << size.x << "," << size.y << ")";

            return ss.str();
        }
}; 

class CCircle : public CShape 
{
    public:
        float radius;
        
        CCircle() {};
        CCircle(
            std::string name, 
            sf::Vector2f pos, 
            sf::Vector2f speed,
            sf::Color color,
            float radius
        ): CShape(name, pos, speed, color), radius(radius) {}

        std::string to_string() const {
            std::ostringstream ss;
            ss  << CShape::to_string()
                << ", radius=" << radius;

            return ss.str();
        }
}; 

class GameConfig
{
    public:
        int window_width;
        int window_height;
        std::string font_file;
        int font_size;
        sf::Color font_color;
        std::vector<std::shared_ptr<CShape>> shapes;
        void load_from(const std::string& file_name) {
            std::ifstream file(file_name);
            std::string head;
            while (file >> head) {
                if (head == "Window") {
                    file >> window_width >> window_height;
                }
                else if (head == "Font") {
                    int r, g, b;
                    file >> font_file >> font_size >> r >> g >> b;
                    font_color = sf::Color(r, g, b);

                }
                else if (head == "Circle") {
                    std::shared_ptr<CCircle> circle = std::make_shared<CCircle>();
                    int r, g, b;
                    file >> circle->name 
                        >> circle->position.x >> circle->position.y 
                        >> circle->speed.x >> circle->speed.y
                        >> r >> g >> b
                        >> circle->radius;
                    circle->color = sf::Color(r, g, b);
                    shapes.push_back(circle);
                }
                else if (head == "Rectangle") {
                    std::shared_ptr<CRectangle> rectangle 
                        = std::make_shared<CRectangle>();
                    int r, g, b;
                    file >> rectangle->name 
                        >> rectangle->position.x >> rectangle->position.y
                        >> rectangle->speed.x >> rectangle->speed.y 
                        >> r >> g >> b
                        >> rectangle->size.x >> rectangle->size.y;
                    rectangle->color = sf::Color(r, g, b);
                    shapes.push_back(rectangle);
                }
                else {
                    std::cerr << "head to " << head << "\n";
                    std::cerr << "The config file format is incorrect!\n";
                    exit(-1);
                }
            }
        }

        std::string to_string() const {
            std::ostringstream ss;
            ss << "Window w=" << window_width << " h=" << window_height << "\n"
                << font_file << " size=" << font_size 
                << " color=("<< (int)font_color.r << "," 
                << (int)font_color.g << "," << (int)font_color.b << ")";

            if (!shapes.empty()) {
                ss << '\n';
                for (auto &shape : shapes) {
                    ss << shape->to_string() << '\n';
                }
            }

            return ss.str();
        }
};

// for debug things
void test_thing(const std::string& str, sf::Text& text, const sf::Font& myFont) {
    text.setFont(myFont);
    text.setString("config.txt content:\n" + str);
    text.setCharacterSize(16);
    auto text_rect = text.getLocalBounds();
    text.setPosition( 10, text_rect.top);
}

int main() {
    // read from config file
    GameConfig config;
    config.load_from("config.txt");

    sf::RenderWindow window(
        sf::VideoMode(config.window_width, config.window_height),
        "SFML works!"
    );
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);
    sf::Clock deltaClock;
    
    // scale the imgui ui
    ImGui::GetStyle().ScaleAllSizes(1.0f);

    // load font
    sf::Font myFont;
    if (!myFont.loadFromFile(config.font_file)) {
        std::cerr << "Could not load font!\n";
        exit(-1);
    }

    sf::Text debug_string; 
    test_thing(config.to_string(), debug_string, myFont);

    // main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // if (event.type == sf::Event::KeyPressed) {
            //     std::cout << "Key pressed with code = " << event.key.code << '\n';

            //     if (event.key.code == sf::Keyboard::X) {
            //         circleSpeedX *= -1.0f;
            //     }
            // }
        }

        // update ImGui for this frame with the time that last frame took
        ImGui::SFML::Update(window, deltaClock.restart());
        // draw UI
        ImGui::Begin("Shape Properties");

        // store our selection data as an index.
        static int shape_idx = 0;         
        // Pass in the preview value visible before opening the combo 
        // (it could be anything)
        const char* combo_preview_value = 
            config.shapes[shape_idx]->name.c_str();  
        if (ImGui::BeginCombo("Shape", combo_preview_value))
        {
            for (int n = 0; n < config.shapes.size(); n++)
            {
                const bool is_selected = (shape_idx == n);
                if (ImGui::Selectable(config.shapes[n]->name.c_str(), is_selected))
                    shape_idx = n;

                // Set the initial focus when opening the combo 
                // (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::Checkbox("Draw Shape", &config.shapes[shape_idx]->draw);
        ImGui::SliderFloat("Scale", &config.shapes[shape_idx]->scale,
            0.0f, 4.0f, "%.3f");
        float velocity[2] { 
            config.shapes[shape_idx]->speed.x,
            config.shapes[shape_idx]->speed.y
        };
        ImGui::SliderFloat2("Velocity", velocity, -8.0f, 8.0f, "%.3f");
        float c[3] {
            (float)config.shapes[shape_idx]->color.r / 255.f,
            (float)config.shapes[shape_idx]->color.g / 255.f,
            (float)config.shapes[shape_idx]->color.b / 255.f
        };
        ImGui::ColorEdit3("Color Circle", c);
        char* shape_name = const_cast<char*>(config.shapes[shape_idx]->name.c_str());
        ImGui::InputText("Name", shape_name, 255);
        ImGui::End();
        
        config.shapes[shape_idx]->speed.x = velocity[0];
        config.shapes[shape_idx]->speed.y = velocity[1];
        config.shapes[shape_idx]->color = sf::Color(c[0]*255, c[1]*255, c[2]*255);
        config.shapes[shape_idx]->name = shape_name;

        window.clear();
        // draw test_thing
        window.draw(debug_string);

        // draw shapes
        for(auto &shape : config.shapes) { 
            sf::Text shape_name(shape->name, myFont, 20);
            shape_name.setFillColor(sf::Color::White);
            shape_name.setScale(shape->scale, shape->scale);
            auto c = std::dynamic_pointer_cast<CCircle>(shape);
            if (c) {
                sf::CircleShape circle(c->radius);
                // collision with walls
                if (c->position.x < 0 || 
                    c->position.x + 2 * c->radius * c->scale > config.window_width) {
                    c->speed.x *= -1;
                }
                if (c->position.y < 0 || 
                    c->position.y + 2 * c->radius * c->scale > config.window_height) {
                    c->speed.y *= -1;
                }
                c->position += c->speed;
                circle.setPosition(c->position);
                circle.setFillColor(c->color);
                circle.setScale(shape->scale, shape->scale);
                // center the text 
                auto circle_rect = circle.getGlobalBounds();
                if (circle_rect.height < 20) {
                    shape_name.setCharacterSize(circle_rect.height * 0.9);
                }
                auto name_rect = shape_name.getGlobalBounds();
                shape_name.setPosition(
                    circle_rect.left + circle_rect.width / 2 - name_rect.width / 2,
                    circle_rect.top + circle_rect.height / 2 - name_rect.height / 2 - 6
                );
                if (c->draw) {
                    window.draw(circle);
                    window.draw(shape_name);
                }
            }
            auto r = std::dynamic_pointer_cast<CRectangle>(shape);
            if (r) {
                sf::RectangleShape rect(r->size);
                // collision with walls
                if (r->position.x < 0 || 
                    r->position.x + r->size.x * r->scale > config.window_width) {
                    r->speed.x *= -1;
                }
                if (r->position.y < 0 || 
                    r->position.y + r->size.y * r->scale > config.window_height) {
                    r->speed.y *= -1;
                }
                r->position += r->speed;
                rect.setPosition(r->position);
                rect.setFillColor(r->color);
                rect.setScale(shape->scale, shape->scale);
                // center the text 
                auto rect_rect = rect.getGlobalBounds();
                if (rect_rect.height < 20) {
                    shape_name.setCharacterSize(rect_rect.height * 0.9);
                }
                auto name_rect = shape_name.getGlobalBounds();
                shape_name.setPosition(
                    rect_rect.left + rect_rect.width / 2 - name_rect.width / 2,
                    rect_rect.top + rect_rect.height / 2 - name_rect.height / 2 - 6
                );
                if (r->draw) {
                    window.draw(rect);
                    window.draw(shape_name);
                }
            }
        }

        ImGui::SFML::Render(window);
        window.display();
    }

    return 0;
}
