#pragma once
#include <lib/egg/heap/heap.hpp>
#include <lib/egg/color_fader.hpp>
#include <types.h>

namespace dReset {

    /// @brief System reset management class.
    class Manage_c {
    public:

        /// @brief The game's possible running states.
        enum MODE_e {
            NORMAL, ///< The game is running regularly.
            SOFT_RESET, ///< The game is being soft reset (from the Home Menu).
            HBM_WAIT, ///< The home menu is open.
            DISK_WAIT, ///< The game cannot read the DVD and cannot proceed.
            FATAL_ERROR, ///< The game cannot read the NAND and cannot proceed.
            SAFETY_WAIT, ///< The game is about to execute the hard reset procedure indicated in ::execMode.
            NO_CHANGE ///< The game's running state hasn't changed [value used in ::modeInit].
        };

        /// @brief The possible hard reset procedures.
        enum EXEC_e {
            RESTART_GAME, ///< Reboots the game's DOL. [Unused].
            REBOOT_SYSTEM, ///< Reboots the entire console. [Unused].
            RETURN_WII_MENU, ///< Returns to the Wii Menu.
            RETURN_DATA_MANAGER, ///< Returns to the Wii Menu's Data Manager.
            POWER_OFF, ///< Turns off the console.
            NONE ///< Does not execute any reset procedure.
        };

        EGG::Heap *mpHeap;
        EGG::ColorFader *mpColorFader;

        int mModeProc; ///< The game's current running state. See ::MODE_e.

        /// @brief The running state the game's about to switch to. See ::MODE_e.
        /// @details The state does not change if this is set to MODE_e::NO_CHANGE.
        int mModeInit;

        int mPrevMode;
        int mExecMode; ///< The hard reset procedure to be executed. See ::EXEC_e.

        bool mHbmReset;
        bool mHbmReturnMenu;

        bool mResetCallbackCalled;
        bool mPowerCallbackCalled;
        bool mInteriorReturnDataManager;

        u32 mHbmResetTypeMaybe;
        u32 mHbmPowerOffTypeMaybe;
        u32 mSoftResetStateMaybe;
        bool mSoftResetPermitted;
        bool mUnknown2D;

        bool mSoftResetRequested;
        bool mFadeOutRequested;
        bool mUnknown30;

        void *mpAudioResetFunc;
        void *mpAudioExitFunc;

        bool mBootCompleted;
        bool mExecutingFade;
        bool mUnknown3E;

        static dReset::Manage_c* GetInstance(); ///< Gets a pointer to the instance of this class.
    };

} // namespace dReset
