template<class T> struct c_ult_reference {
	c_ult_reference* next;
	c_ult_reference* prev;
	T* object;
};
template<class T> struct c_ult_intrusive_list {
	T* head;
};
template<class T> struct c_ulti_intrusive_dlist : public c_ult_intrusive_list<T> {};
template<class T> struct c_ult_reference_list : public c_ulti_intrusive_dlist< c_ult_reference<T> > {};
 
enum e_attribute_data_trype {
	ATTRDATATYPE_NONE = -1,
	ATTRDATATYPE_FLOAT = 0,
	ATTRDATATYPE_4V,
	ATTRDATATYPE_INT,
	ATTRDATATYPE_POINTER,
 
	ATTRDATATYPE_COUNT,
};
 
#define MAX_PARTICLE_ATTRIBUTES 24
 
#define DEFPARTICLE_ATTRIBUTE( name, bit, datatype )			\
	const int PARTICLE_ATTRIBUTE_##name##_MASK = (1 << bit);	\
	const int PARTICLE_ATTRIBUTE_##name = bit;					\
	const e_attribute_data_trype PARTICLE_ATTRIBUTE_##name##_DATATYPE = datatype;
 
DEFPARTICLE_ATTRIBUTE(TINT_RGB, 6, ATTRDATATYPE_4V);
 
DEFPARTICLE_ATTRIBUTE(ALPHA, 7, ATTRDATATYPE_FLOAT);
 
struct particle_attribute_address_table {
	float* m_pAttributes[MAX_PARTICLE_ATTRIBUTES];
	size_t m_nFloatStrides[MAX_PARTICLE_ATTRIBUTES];
 
	FORCEINLINE float* float_attribute_pointer(int nAttribute, int nParticleNumber) const {
		int block_ofs = nParticleNumber / 4;
		return m_pAttributes[nAttribute] +
			m_nFloatStrides[nAttribute] * block_ofs +
			(nParticleNumber & 3);
	}
 
};
 
struct ult_string_simple {
	char* buffer;
	int capacity;
	int grow_size;
	int length;
};
 
class particle_system_definition {
	BYTE pad_0[308];
public:
	ult_string_simple name;
};
 
class particle_collection {
	BYTE pad_0[48];//0
public:
	int active_particle;//48
private:
	BYTE pad_1[12];//52
public:
	c_ult_reference<particle_system_definition> definition;//64
private:
	BYTE pad_2[60];//80
public:
	particle_collection* parent;//136
private:
	BYTE pad_3[84];//140
public:
	particle_attribute_address_table particle_attribute;//224
};
