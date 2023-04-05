from ghcr.io/wiiu-env/devkitppc:20220806

# PC build dependencies
RUN apt-get update && apt-get install -y \
    gcc g++ libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libsdl2-gfx-dev

WORKDIR project
CMD make