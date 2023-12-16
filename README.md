# a1-4300
generate shapes and bouncing

dependencies:
- SFML 2.6.1
- DearImGui 1.90
- ImGui-SFML 2.6

ImGui is able to:
- list all of the shapes and select any of them to edit their properties
- toggle whether or not the shapes and selectd shape will be drawn
- change the scale of the shape (0 to 4)
- change the x and y velocity of the shape (-8 to 8)
- change the color of the shape
- change the name of the shape

config.txt <br>
Window W H <br>

Font F S R G B
- Font File F std::string
- Font Size S int
- RGB Color (R, G, B) int, int, int

Rectangle N X Y SX SY R G B W H
- Shape Name Name std::string
- Initial Position (X, Y) float, float
- Initial Speed (SX, SY) float, float
- RGB Color (R, G, B) int, int, int
- Size (W, H) float, float

Circle N X Y SX SY R G B R
- Shape Name Name std::string
- Initial Position (X, Y) float, float
- Initial Speed (SX, SY) float, float
- RGB Color (R, G, B) int, int, int
- Radius R float, float
<img width="1392" alt="Screenshot 2023-12-16 at 22 08 08" src="https://github.com/kin789246/a1-4300-cpp/assets/30062348/03baba99-bef5-4b7a-8241-5d94ca2cf5ec">
