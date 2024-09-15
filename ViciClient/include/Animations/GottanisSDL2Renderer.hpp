#include <SDL2/SDL.h>
#include <gottanis/include/IRenderer.h>

namespace Animations {
class GottanisSDL2Renderer : public gottanis::IRenderer {
public:
    GottanisSDL2Renderer(SDL_Renderer *renderer);
    virtual ~GottanisSDL2Renderer() override = default;

    virtual void getTextureSize(gottanis::TextureHandle texture, int* w, int* h) override;
    virtual void clearRect(int x, int y, int w, int h, gottanis::TextureHandle atlasTexture) override;
    virtual void mergeTexture(int x, int y, int w, int h, gottanis::TextureHandle childTexture, gottanis::TextureHandle atlasTexture) override;
    virtual gottanis::TextureHandle createEmptyTexture(int width, int height) override;
    virtual gottanis::TextureHandle loadTexture(std::string_view path) override;
    virtual void unloadTexture(gottanis::TextureHandle texture) override;
    virtual void render(gottanis::TextureHandle tex, gottanis::Rect* src, gottanis::RectF* dest = nullptr, float rotation = 0.0f, gottanis::PointF* pivot = nullptr, gottanis::RendererFlip flip = gottanis::RendererFlip::NONE) override;
    virtual gottanis::TextureHandle createRenderTarget(int width, int height) override;

private:
    SDL_Renderer *_renderer;
};
} // namespace Animations