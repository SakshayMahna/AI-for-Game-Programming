#include "animation.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <map>
#include <iostream>

// Texture Manager
void TextureManager::load_texture(const std::string &name, const std::string &filename) {
    // Load texture and add to list
    sf::Texture texture;
    texture.loadFromFile(filename);
    textures[name] = texture;

    return;
}

sf::Texture &TextureManager::getRef(const std::string &texture) {
    return textures.at(texture);
}

// Animation Manager
Animation::Animation(unsigned int start, unsigned int end, float i_duration) {
    start_frame = start;
    end_frame = end;
    duration = i_duration;
}

AnimationHandler::AnimationHandler() {
    t = 0.0f;
    current_animation = -1;
}

AnimationHandler::AnimationHandler(const sf::IntRect &frame) {
    frame_size = frame;
    t = 0.0f;
    current_animation = -1;
}

void AnimationHandler::update(const float dt) {
    if (current_animation >= animations.size() || current_animation < 0)
        return;

    float duration = animations[current_animation].duration;

    // Progress animation to next frame
    if(int((t + dt) / duration) > int(t / duration)) {
        int frame = int((t + dt) / duration);
        frame = frame % animations[current_animation].get_length();

        sf::IntRect rect = frame_size;
        rect.left = rect.width * frame;
        rect.top = rect.height * current_animation;
        bounds = rect;
    }

    // Increment the time elapsed
    t = t + dt;
    if (t > duration * animations[current_animation].get_length())
        t = 0.0f;
    
    return;
}

void AnimationHandler::add_animation(Animation &animation) {
    animations.push_back(animation);
}

void AnimationHandler::change_animation(unsigned int animation_number) {
    if (current_animation == animation_number || 
        animation_number >= animations.size() ||
        animation_number < 0)
        return;

    current_animation = animation_number;
    
    sf::IntRect rect = frame_size;
    rect.top = rect.height * animation_number;
    bounds = rect;
    t = 0.0f;

    return;
}