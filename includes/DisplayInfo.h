struct DisplayInfo {
    int width;
    int height;
    bool resizable;
    enum WindowMode { Windowed, Borderless, Fullscreen } mode;
    int fpsCap;

    DisplayInfo(int w, int h, bool res, WindowMode m, int fps)
        : width(w), height(h), resizable(res), mode(m), fpsCap(fps) {}
};