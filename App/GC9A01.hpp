/**
 * @file GC9A01.hpp
 * @author KUSAKABE Yuki (yuqlid@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-02-05
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef GC9A01_HPP_
#define GC9A01_HPP_

#include <cstdint>
#include <cstddef>
// Hardware abstraction layer
// Should be defined by the user of the library
void GC9A01_set_reset(uint8_t val);
void GC9A01_set_data_command(uint8_t val);
void GC9A01_set_chip_select(uint8_t val);

struct GC9A01_point {
  uint16_t X, Y;
};

struct GC9A01_frame {
  struct GC9A01_point start, end;
};

void GC9A01_init(void);
void GC9A01_set_frame(struct GC9A01_frame frame);
void GC9A01_write(uint8_t *data, size_t len);
void GC9A01_write_continue(uint8_t *data, size_t len);

void GC9A01_write_data(uint8_t *data, size_t len);
void GC9A01_write_command(uint8_t cmd);

#endif /* GC9A01_HPP_ */