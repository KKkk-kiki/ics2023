#ifndef __DEVICE_H__
#define __DEVICE_H__
size_t serial_write(const void *buf, size_t offset, size_t len);

size_t events_read(void *buf, size_t offset, size_t len);

size_t dispinfo_read(void *buf, size_t offset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);
size_t am_ioe_read(void *buf, size_t offset, size_t len);
size_t am_ioe_write(const void *buf, size_t offset, size_t len);
void init_device();


#endif