#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <map>

// Texture Management
class TextureManager {
private:
    // Array of textures to use
    std::map<std::string, sf::Texture> textures;

public:
    TextureManager() = default;

    // Add texture from a file
    void load_texture(const std::string &name, const std::string &filename);

    // Translate an id to reference
    sf::Texture &getRef(const std::string &texture);
};

// Animation Management
class Animation {
public:
    unsigned int start_frame;
    unsigned int end_frame;
    float duration;

    Animation();
    Animation(unsigned int start, unsigned int end, float i_duration);
    unsigned int get_length() { return end_frame - start_frame + 1; };
};

class AnimationHandler {
private:
    // Array of animations
    std::vector<Animation> animations;

    // Current time since animation loop started
    float t;
    int current_animation;

public:
    // Current section of texture
    sf::IntRect bounds;
    sf::IntRect frame_size;

    AnimationHandler();
    AnimationHandler(const sf::IntRect &frame);

    // Add new animation
    void add_animation(Animation &animation);
    // Update current frame of animation
    void update(const float dt);
    // Change animation, reset t
    void change_animation(unsigned int animation_number);
};

#endif