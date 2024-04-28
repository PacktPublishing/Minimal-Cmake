// third-party dependencies
#include <as-ops.h>
#include <bgfx/c99/bgfx.h>
#include <minimal-cmake-gol/gol.h>
#include <minimal-cmake/array.h>
#include <timer.h>

// system includes
#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include <SDL_syswm.h>

typedef struct color_t {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} color_t;

typedef struct pos_color_vertex_t {
  float x;
  float y;
  float z;
  uint32_t abgr;
} pos_color_vertex_t;

static pos_color_vertex_t quad_vertices[] = {
  {-1.0f, -1.0f, 0.0f, 0xffffffff},
  {1.0f, -1.0f, 0.0f, 0xffffffff},
  {-1.0f, 1.0f, 0.0f, 0xffffffff},
  {1.0f, 1.0f, 0.0f, 0xffffffff}};

static const uint16_t quad_indices[] = {0, 1, 2, 1, 3, 2};

static char* read_file(const char* filepath) {
  FILE* file = fopen(filepath, "rb");
  fseek(file, 0, SEEK_END);
  int64_t file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char* buffer = NULL;
  mc_array_resize(buffer, file_size);

  size_t bytes_read = fread(buffer, sizeof(char), file_size, file);

  int s = mc_array_size(buffer);

  return buffer;
}

static bgfx_shader_handle_t create_shader(
  const char* shader, const int size, const char* name) {
  const bgfx_memory_t* mem = bgfx_copy(shader, size);
  const bgfx_shader_handle_t handle = bgfx_create_shader(mem);
  bgfx_set_shader_name(handle, name, strlen(name));
  return handle;
}

double seconds_elapsed(
  const uint64_t previous_counter, const uint64_t current_counter) {
  return (double)(current_counter - previous_counter)
       / (double)SDL_GetPerformanceFrequency();
}

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(
      stderr, "SDL could not initialize. SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  const as_vec2i screen_dimensions = (as_vec2i){.x = 800, .y = 600};
  SDL_Window* window = SDL_CreateWindow(
    argv[0], SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    screen_dimensions.x, screen_dimensions.y, SDL_WINDOW_SHOWN);

  if (window == NULL) {
    fprintf(
      stderr, "Window could not be created. SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_SysWMinfo wmi;
  SDL_VERSION(&wmi.version);
  if (!SDL_GetWindowWMInfo(window, &wmi)) {
    fprintf(
      stderr, "SDL_SysWMinfo could not be retrieved. SDL_Error: %s\n",
      SDL_GetError());
    return 1;
  }
  bgfx_render_frame(-1); // single threaded mode

  bgfx_platform_data_t pd = {};
#if BX_PLATFORM_WINDOWS
  pd.nwh = wmi.info.win.window;
#elif BX_PLATFORM_OSX
  pd.nwh = wmi.info.cocoa.window;
#elif BX_PLATFORM_LINUX
  pd.ndt = wmi.info.x11.display;
  pd.nwh = (void*)(uintptr_t)wmi.info.x11.window;
#endif

  bgfx_init_t bgfx;
  bgfx_init_ctor(&bgfx);
  bgfx.type = BGFX_RENDERER_TYPE_COUNT; // auto choose renderer
  bgfx.resolution.width = screen_dimensions.x;
  bgfx.resolution.height = screen_dimensions.y;
  bgfx.resolution.reset = BGFX_RESET_VSYNC;
  bgfx.platformData = pd;
  bgfx_init(&bgfx);

  mc_gol_board_t* board = mc_gol_create_board(40, 27);

  // gosper glider gun
  mc_gol_set_board_cell(board, 2, 5, true);
  mc_gol_set_board_cell(board, 2, 6, true);
  mc_gol_set_board_cell(board, 3, 5, true);
  mc_gol_set_board_cell(board, 3, 6, true);

  mc_gol_set_board_cell(board, 12, 5, true);
  mc_gol_set_board_cell(board, 12, 6, true);
  mc_gol_set_board_cell(board, 12, 7, true);
  mc_gol_set_board_cell(board, 13, 4, true);
  mc_gol_set_board_cell(board, 13, 8, true);
  mc_gol_set_board_cell(board, 14, 3, true);
  mc_gol_set_board_cell(board, 14, 9, true);
  mc_gol_set_board_cell(board, 15, 3, true);
  mc_gol_set_board_cell(board, 15, 9, true);
  mc_gol_set_board_cell(board, 16, 6, true);
  mc_gol_set_board_cell(board, 17, 4, true);
  mc_gol_set_board_cell(board, 17, 8, true);
  mc_gol_set_board_cell(board, 18, 5, true);
  mc_gol_set_board_cell(board, 18, 6, true);
  mc_gol_set_board_cell(board, 18, 7, true);
  mc_gol_set_board_cell(board, 19, 6, true);

  mc_gol_set_board_cell(board, 22, 3, true);
  mc_gol_set_board_cell(board, 22, 4, true);
  mc_gol_set_board_cell(board, 22, 5, true);
  mc_gol_set_board_cell(board, 23, 3, true);
  mc_gol_set_board_cell(board, 23, 4, true);
  mc_gol_set_board_cell(board, 23, 5, true);
  mc_gol_set_board_cell(board, 24, 2, true);
  mc_gol_set_board_cell(board, 24, 6, true);
  mc_gol_set_board_cell(board, 26, 1, true);
  mc_gol_set_board_cell(board, 26, 2, true);
  mc_gol_set_board_cell(board, 26, 6, true);
  mc_gol_set_board_cell(board, 26, 7, true);

  mc_gol_set_board_cell(board, 36, 3, true);
  mc_gol_set_board_cell(board, 36, 4, true);
  mc_gol_set_board_cell(board, 37, 3, true);
  mc_gol_set_board_cell(board, 37, 4, true);

  // eater
  mc_gol_set_board_cell(board, 27, 20, true);
  mc_gol_set_board_cell(board, 27, 21, true);
  mc_gol_set_board_cell(board, 28, 20, true);
  mc_gol_set_board_cell(board, 28, 21, true);

  mc_gol_set_board_cell(board, 32, 21, true);
  mc_gol_set_board_cell(board, 31, 22, true);
  mc_gol_set_board_cell(board, 33, 22, true);
  mc_gol_set_board_cell(board, 32, 23, true);

  mc_gol_set_board_cell(board, 34, 23, true);
  mc_gol_set_board_cell(board, 34, 24, true);
  mc_gol_set_board_cell(board, 34, 25, true);
  mc_gol_set_board_cell(board, 35, 25, true);

  bgfx_vertex_layout_t pos_col_vert_layout;
  bgfx_vertex_layout_begin(&pos_col_vert_layout, BGFX_RENDERER_TYPE_COUNT);
  bgfx_vertex_layout_add(
    &pos_col_vert_layout, BGFX_ATTRIB_POSITION, 3, BGFX_ATTRIB_TYPE_FLOAT,
    false, false);
  bgfx_vertex_layout_add(
    &pos_col_vert_layout, BGFX_ATTRIB_COLOR0, 4, BGFX_ATTRIB_TYPE_UINT8, true,
    false);
  bgfx_vertex_layout_end(&pos_col_vert_layout);

  bgfx_vertex_buffer_handle_t vbh = bgfx_create_vertex_buffer(
    bgfx_make_ref(quad_vertices, sizeof(quad_vertices)), &pos_col_vert_layout,
    0);
  bgfx_index_buffer_handle_t ibh = bgfx_create_index_buffer(
    bgfx_make_ref(quad_indices, sizeof(quad_indices)), 0);

  // shader stuff
  char* vs_shader = read_file("shader/build/vs_vertcol.bin");
  char* fs_shader = read_file("shader/build/fs_vertcol.bin");

  bgfx_shader_handle_t vsh =
    create_shader(vs_shader, mc_array_size(vs_shader), "vs_shader");
  bgfx_shader_handle_t fsh =
    create_shader(fs_shader, mc_array_size(fs_shader), "fs_shader");
  bgfx_program_handle_t program = bgfx_create_program(vsh, fsh, true);

  mc_array_free(vs_shader);
  mc_array_free(fs_shader);

  double timer = 0.0;
  const double delay = 0.1f;
  double previous_frame_time = SDL_GetPerformanceFrequency();
  for (bool running = true; running;) {
    for (SDL_Event current_event; SDL_PollEvent(&current_event) != 0;) {
      if (current_event.type == SDL_QUIT) {
        running = false;
        break;
      }
    }

    const int64_t current_counter = SDL_GetPerformanceCounter();
    const double delta_time =
      seconds_elapsed(previous_frame_time, current_counter);
    previous_frame_time = current_counter;

    timer += delta_time;

    // clear screen
    bgfx_set_view_clear(
      0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x6495EDFF, 1.0f, 0);
    bgfx_set_view_rect(0, 0, 0, screen_dimensions.x, screen_dimensions.y);

    as_mat44f identity = as_mat44f_identity();

    const float aspect_ratio =
      (float)screen_dimensions.x / (float)screen_dimensions.y;
    as_mat44f projection =
      as_mat44f_orthographic_projection_depth_zero_to_one_lh(
        -50.0 * aspect_ratio, 50.0 * aspect_ratio, -50.0, 50.0, 0.0, 1.0);

    bgfx_set_view_transform(0, identity.elem, projection.elem);

    bgfx_set_transform(identity.elem, 1);

    bgfx_set_vertex_buffer(0, vbh, 0, 4);
    bgfx_set_index_buffer(ibh, 0, 6);

    bgfx_submit(0, program, 0, BGFX_DISCARD_ALL);

    const float cell_size = 15;
    const as_point2i board_top_left_corner = (as_point2i){
      .x = (screen_dimensions.x / 2)
         - (mc_gol_board_width(board) * cell_size) * 0.5f,
      .y = (screen_dimensions.y / 2)
         - (mc_gol_board_height(board) * cell_size) * 0.5f};
    for (int32_t y = 0, height = mc_gol_board_height(board); y < height; y++) {
      for (int32_t x = 0, width = mc_gol_board_width(board); x < width; x++) {
        const as_point2i cell_position = as_point2i_add_vec2i(
          board_top_left_corner,
          (as_vec2i){.x = x * cell_size, .y = y * cell_size});
        const color_t cell_color =
          mc_gol_board_cell(board, x, y)
            ? (color_t){.r = 255, .g = 255, .b = 255, .a = 255}
            : (color_t){.a = 255};
        // SDL_SetRenderDrawColor(
        //   renderer, cell_color.r, cell_color.g, cell_color.b, cell_color.a);
        const SDL_Rect cell = (SDL_Rect){
          .x = cell_position.x,
          .y = cell_position.y,
          .w = cell_size,
          .h = cell_size};
        // SDL_RenderFillRect(renderer, &cell);
      }
    }

    if (timer > delay) {
      mc_gol_update_board(board);
      timer = 0.0;
    }

    // SDL_RenderPresent(renderer);
    bgfx_touch(0);
    bgfx_frame(false);
  }

  mc_gol_destroy_board(board);

  bgfx_shutdown();
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
