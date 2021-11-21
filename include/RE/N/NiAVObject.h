#pragma once

#include "RE/B/BSFixedString.h"
#include "RE/B/BSLightingShaderProperty.h"
#include "RE/B/BSShaderMaterial.h"
#include "RE/N/NiBound.h"
#include "RE/N/NiObjectNET.h"
#include "RE/N/NiSmartPointer.h"
#include "RE/N/NiTransform.h"

namespace RE
{
	class NiAlphaProperty;
	class NiAVObject;
	class NiCollisionObject;
	class NiColor;
	class NiColorA;
	class NiCullingProcess;
	class NiNode;
	class NiPoint3;
	class TESObjectREFR;
	class BSGeometry;

	class NiUpdateData
	{
	public:
		enum class Flag
		{
			kNone = 0,
			kDirty = 1 << 0,
			kDisableCollision = 8193
		};

		float                                 time;   // 0
		stl::enumeration<Flag, std::uint32_t> flags;  // 4
	};
	static_assert(sizeof(NiUpdateData) == 0x8);

	class PerformOpFunc
	{
	public:
		virtual ~PerformOpFunc();  // 00

		// add
		virtual bool operator()(NiAVObject* a_object);  // 01
	};
	static_assert(sizeof(PerformOpFunc) == 0x8);

	class NiAVObject : public NiObjectNET
	{
	public:
		inline static constexpr auto RTTI = RTTI_NiAVObject;
		inline static constexpr auto Ni_RTTI = NiRTTI_NiAVObject;

		enum class Flag
		{
			kNone = 0,
			kHidden = 1 << 0,
			kSelectiveUpdate = 1 << 1,
			kSelectiveUpdateTransforms = 1 << 2,
			kSelectiveUpdateController = 1 << 3,
			kSelectiveUpdateRigid = 1 << 4,
			kDisplayObject = 1 << 5,
			kDisableSorting = 1 << 6,
			kSelectiveUpdateTransformsOverride = 1 << 7,
			kSaveExternalGeometryData = 1 << 9,
			kNoDecals = 1 << 10,
			kAlwaysDraw = 1 << 11,
			kMeshLOD = 1 << 12,
			kFixedBound = 1 << 13,
			kTopFadeNode = 1 << 14,
			kIgnoreFade = 1 << 15,
			kNoAnimSyncX = 1 << 16,
			kNoAnimSyncY = 1 << 17,
			kNoAnimSyncZ = 1 << 18,
			kNoAnimSyncS = 1 << 19,
			kNoDismember = 1 << 20,
			kNoDismemberValidity = 1 << 21,
			kRenderUse = 1 << 22,
			kMaterialsApplied = 1 << 23,
			kHighDetail = 1 << 24,
			kForceUpdate = 1 << 25,
			kPreProcessedNode = 1 << 26
		};

		~NiAVObject() override;  // 00

		// override (NiObjectNET)
		const NiRTTI* GetRTTI() const override;                            // 02
		void          LoadBinary(NiStream& a_stream) override;             // 18
		void          LinkObject(NiStream& a_stream) override;             // 19
		bool          RegisterStreamables(NiStream& a_stream) override;    // 1A
		void          SaveBinary(NiStream& a_stream) override;             // 1B
		bool          IsEqual(NiObject* a_object) override;                // 1C
		void          ProcessClone(NiCloningProcess& a_cloning) override;  // 1D

		// add
		virtual void UpdateControllers(NiUpdateData& a_data);  // 25
#ifdef SKYRIMVR
		virtual void Unk_VRFunc(void);
#endif
		virtual void        PerformOp(PerformOpFunc& a_func);                                                                   // 26
		virtual void        AttachProperty(NiAlphaProperty* a_property);                                                        // 27 - { return; }
		virtual void        SetMaterialNeedsUpdate(bool a_needsUpdate);                                                         // 28 - { return; }
		virtual void        SetDefaultMaterialNeedsUpdateFlag(bool a_flag);                                                     // 29 - { return; }
		virtual NiAVObject* GetObjectByName(const BSFixedString& a_name);                                                       // 2A
		virtual void        SetSelectiveUpdateFlags(bool& a_selectiveUpdate, bool a_selectiveUpdateTransforms, bool& a_rigid);  // 2B
		virtual void        UpdateDownwardPass(NiUpdateData& a_data, std::uint32_t a_arg2);                                     // 2C
		virtual void        UpdateSelectedDownwardPass(NiUpdateData& a_data, std::uint32_t a_arg2);                             // 2D
		virtual void        UpdateRigidDownwardPass(NiUpdateData& a_data, std::uint32_t a_arg2);                                // 2E
		virtual void        UpdateWorldBound();                                                                                 // 2F - { return; }
		virtual void        UpdateWorldData(NiUpdateData* a_data);                                                              // 30
		virtual void        UpdateTransformAndBounds(NiUpdateData& a_data);                                                     // 31
		virtual void        PreAttachUpdate(NiNode* a_parent, NiUpdateData& a_data);                                            // 32
		virtual void        PostAttachUpdate();                                                                                 // 33
		virtual void        OnVisible(NiCullingProcess& a_process);                                                             // 34 - { return; }

		void                         ClearDecals();
		void                         ClearWeaponBlood();
		void                         CullNode(bool a_cull);
		[[nodiscard]] bool           GetAppCulled() const;
		[[nodiscard]] BSGeometry*    GetFirstGeometryOfShaderType(BSShaderMaterial::Feature a_type);
		[[nodiscard]] TESObjectREFR* GetUserData() const;
		[[nodiscard]] bool           HasShaderType(BSShaderMaterial::Feature a_type);
		void                         SetAppCulled(bool a_cull);
		bool                         SetMotionType(std::uint32_t a_motionType, bool a_arg2 = true, bool a_arg3 = false, bool a_allowActivate = true);
		void                         SetRigidConstraints(bool a_enable, std::uint8_t a_arg2 = 1, std::uint32_t a_arg3 = 1);
		void                         TintScenegraph(const NiColorA& a_color);
		void                         Update(NiUpdateData& a_data);
		void                         UpdateBodyTint(const NiColor& a_color);
		void                         UpdateHairColor(const NiColor& a_color);
		void                         UpdateMaterialAlpha(float a_alpha, bool a_doOnlySkin);
		void                         UpdateMaterialShader(const NiColorA& a_projectedUVParams, const NiColor& a_projectedUVColor, const bool a_isSnow);
		void                         UpdateRigidBodySettings(std::uint32_t a_type, std::uint32_t a_arg2);

		BSLightingShaderProperty* temp_nicast(BSGeometry* a_geometry);

		// members
#ifndef SKYRIMVR
		NiNode*                               parent;                   // 030
		std::uint32_t                         parentIndex;              // 038
		std::uint32_t                         unk03C;                   // 03C
		NiPointer<NiCollisionObject>          collisionObject;          // 040
		NiTransform                           local;                    // 048
		NiTransform                           world;                    // 07C
		NiTransform                           previousWorld;            // 0B0
		NiBound                               worldBound;               // 0E4
		stl::enumeration<Flag, std::uint32_t> flags;                    // 0F4
		TESObjectREFR*                        userData;                 // 0F8
		float                                 fadeAmount;               // 100
		std::uint32_t                         lastUpdatedFrameCounter;  // 104
		std::uint64_t                         unk108;                   // 108
	};
	static_assert(sizeof(NiAVObject) == 0x110);
#else
		NiNode*                               parent;                   // 030
		std::uint32_t                         parentIndex;              // 038
		std::uint32_t                         unk03C;                   // 03C
		NiPointer<NiCollisionObject>          collisionObject;          // 040
		NiTransform                           local;                    // 048
		NiTransform                           world;                    // 07C
		NiTransform                           previousWorld;            // 0B0
		NiBound                               worldBound;               // 0E4
		float                                 unkF4;                    // 0F4
		float                                 unkF8;                    // 0F8
		float                                 unkFC;                    // 0FC
		float                                 fadeAmount;               // 100
		std::uint32_t                         lastUpdatedFrameCounter;  // 104
		float                                 unk108;                   // 108
		stl::enumeration<Flag, std::uint32_t> flags;                    // 10C
		TESObjectREFR*                        userData;                 // 110
		std::uint32_t                         unk11C;                   // 11C
		std::uint8_t                          unk120[8];                // 120 - bitfield
		std::uint64_t                         unk128;                   // 128
		std::uint32_t                         unk130;                   // 130
		std::uint32_t                         unk134;                   // 134
	};
	static_assert(sizeof(NiAVObject) == 0x138);
#endif
}
