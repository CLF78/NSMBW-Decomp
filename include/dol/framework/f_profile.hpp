#pragma once
#include <types.h>
#include <dol/framework/f_profile_name.hpp>
/// @file

/// @addtogroup profile
/// @{

/// @brief Creates a profile of a base with given values for execute and draw order.
/// @hideinitializer
#define SPECIAL_BASE_PROFILE(profName, className, executeOrder, drawOrder) void *className##_classInit() { return new className(); } \
    fProfile::fBaseProfile_c g_profile_##profName = { &className##_classInit, executeOrder, drawOrder }

/// @brief Creates a profile of an actor with given values for execute and draw order and the actor properties.
/// @hideinitializer
/// @see SPECIAL_BASE_PROFILE
#define SPECIAL_ACTOR_PROFILE(profName, className, executeOrder, drawOrder, properties) void *className##_classInit() { return new className(); } \
    const fProfile::fActorProfile_c g_profile_##profName = { &className##_classInit, executeOrder, drawOrder, properties }

/// @brief Creates a profile for a base, with the profile number as the priority for both the draw and execute order.
/// @hideinitializer
/// @see SPECIAL_BASE_PROFILE
#define DEFAULT_BASE_PROFILE(profName, className) SPECIAL_BASE_PROFILE(profName, className, fProfile::profName, fProfile::profName);

/// @brief Creates a profile of an actor with default values.
/// @hideinitializer
/// @see DEFAULT_BASE_PROFILE
#define DEFAULT_ACTOR_PROFILE(profName, className, properties) SPECIAL_ACTOR_PROFILE(profName, className, fProfile::profName, fProfile::profName, properties);

/// @brief The name of a profile. Value is a fProfile::PROFILE_NAME_e.
/// @note The compilation order suggests that this file might have been grouped together
/// with the rest of framework, so a similar naming scheme has been applied here.
typedef u16 ProfileName;

/**
 * @brief Gets a string representing the profile name.
 * @ingroup dol
 * @param profName The profile name for which to get the name string of.
 * @return A string containing the profile name.
 */
char *dProf_getName(ProfileName profName);

/// @brief For all profile related structures.
/// @unofficial
namespace fProfile {

    /// @brief A set of basic information needed to construct a base.
    /// @details A profile consists of a pointer to a constructor function
    /// and a priority value for execution and drawing order.
    struct fBaseProfile_c {
        void *(*mpClassInit)(); ///< The constructor function.
        u16 mExecuteOrder; ///< The execution priority of the base.
        u16 mDrawOrder; ///< The draw priority of the base.
    };

    /// @brief A set of basic information needed to construct an actor.
    /// @details In addition to the fields in fBaseProfile_c, it also contains some properties about the actor.
    struct fActorProfile_c : fBaseProfile_c {
        u32 mActorProperties; ///< Some actor-related properties. @todo Document the bitfield.
    };

    extern fBaseProfile_c *(*sProfileList)[PROFILE_COUNT]; ///< A list of all profiles. @ingroup dol

} // namespace fProfile

/// @}
