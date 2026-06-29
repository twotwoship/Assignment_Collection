/*
	level 0 discovery
	SHALL
	level 0 discovery header
	tper feature descriptor
	locking feature descriptor
	opal SSC V2 feature descriptor

	MAY
	geometry reporting feature descriptor
	supported data removal mechanism feature descriptor

*/

typedef struct {
	unsigned int Length_of_Parameter_Data;	// VU
	unsigned int Data_structure_revision;	// 0x00000001 or any version
	unsigned int Reserved[2];
	unsigned int Vendor_Specific[8];		// VU
}Level_0_Discovery_Header;

typedef struct {
	unsigned short Feature_Code;			// 0x0001
	unsigned char Version_Reserved;			// 0x1 or any version
	unsigned char Length;					// 0x0c
	unsigned char Capability_Flags;			// 
	unsigned char Reserved[10];
}Level_0_Discovery_TPer_Feature_Descriptor;


