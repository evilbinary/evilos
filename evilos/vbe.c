#include <multiboot.h>
#include <vbe.h>

extern multiboot_info_t *multiboot_info;

vbe_mode_t* get_vbe_mode_info(){
	return  (vbe_mode_t*)multiboot_info->vbe_mode_info;
}
int get_vbe_mode(){
	return multiboot_info->vbe_mode;
}
vbe_controller_t * get_vbe_controller_info(){
	return (vbe_controller_t *)multiboot_info->vbe_control_info;
}






