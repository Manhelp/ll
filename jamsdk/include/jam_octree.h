
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			OCTree Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_OCTREE_H__
#define __JAM_OCTREE_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t				octree_t;


	/** ����һ���˲��� */
	JAM_API octree_t jam_octree_alloc(float max_x, float max_y, float max_z);


	/** �ͷ�һ���˲��� */
	JAM_API void jam_octree_free(octree_t oct);


	/** ������ķ�ʽ�����޳� */
	JAM_API size_t jam_octree_cull_sphere(octree_t oct, uint64_t* list, size_t len, const sphere_t* s);


	/** �԰�Χ�еķ�ʽ�����޳� */
	JAM_API size_t jam_octree_cull_box(octree_t oct, uint64_t* list, size_t len, const box_t* b);


	/** ��ƽ��׵��ķ�ʽ�����޳� */
	JAM_API size_t jam_octree_cull_frustum(octree_t oct, uint64_t* list, size_t len, const frustum_t* f);


	/** ���˲����з���һ������ */
	JAM_API uint64_t jam_octree_obj_put(octree_t oct, float x, float y, float z, void* obj);


	/** ͨ���������Ż�ȡ���� */
	JAM_API void* jam_octree_obj_get(octree_t oct, uint64_t id);


	/** ͨ����������ɾ������ */
	JAM_API bool jam_octree_obj_erase(octree_t oct, uint64_t id);


	/** �ƶ�һ������ */
	JAM_API bool jam_octree_obj_move(octree_t oct, uint64_t id, float x, float y, float z);


	/** ��ȡ���� x ���� */
	JAM_API float jam_octree_obj_x(octree_t oct, uint64_t id);


	/** ��ȡ���� y ���� */
	JAM_API float jam_octree_obj_y(octree_t oct, uint64_t id);


	/** ��ȡ���� z ���� */
	JAM_API float jam_octree_obj_z(octree_t oct, uint64_t id);


	/** ��ȡ�˲����������� */
	JAM_API size_t jam_octree_obj_count(octree_t oct);


	/** ��ȡ�˲����ڵ����� */
	JAM_API size_t jam_octree_node_count(octree_t oct);


	/** ��ȡ�˲�����ǰ��� */
	JAM_API uint16_t jam_octree_depth(octree_t oct);


	/** ��ȡ�˲������ x ���� */
	JAM_API float jam_octree_max_x(octree_t oct);


	/** ��ȡ�˲������ y ���� */
	JAM_API float jam_octree_max_y(octree_t oct);


	/** ��ȡ�˲������ z ���� */
	JAM_API float jam_octree_max_z(octree_t oct);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_OCTREE_H__ */
