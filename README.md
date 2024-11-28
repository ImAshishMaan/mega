# mega

NOTE: Everything is in C++. And this doesn't contain any environment.

This is a comprehensive learning project designed to enhance my skills in Unreal Engine game development. The project features a wide array of mechanics and systems, each built with extensibility and performance in mind.

Key Features:
1. Advanced Locomotion System
    Full animation set, including start, cycle, stop, pivot, turn-in-place, jump, and foot placements.
    Smooth transitions and realistic movements for enhanced gameplay.

2. Animation Layer System
    Modular setup to support weapon-specific animations (e.g., pistol, rifle).
    Easily extendable to incorporate new weapons and animations.

3. Aiming System
    Aim-down-sights functionality with a customizable aim icon.
    Adjusts based on the currently equipped weapon.

4. First-Person and Third-Person Views
    Integrated FPP and TPP modes.
    FPP animations need further refinement for smoother transitions.

5. Weapon Mechanics
    Fire, reload, equip, unequip, drop, and holster functionality.
    Bullet decals, fracturable geometry, and surface-based particle effects.
    Support for both tracer and normal projectile types.

6. Magic Ability System
    Fire magic projectiles when no weapon is equipped.
    Uses GameplayTags for easy extension and new ability additions.

7. Character Overlay Widgets
    Health widget driven by an independent UAttributeComponent.

8. AI Enemies
    Behavior tree-driven AI with shooting mechanics and health components.
    Displays damage values and a health bar when hit.
    Basic implementation, with room for death effects and further behavior.

9. Pickup and Healing System
    A pickup class that regenerates health.
    Usable by both player and AI upon overlap.

10. Animal AI
    A roaming animal AI using behavior trees.
    Seeks and consumes nearby grass when hungry.

11. UI Stack System
    Pause and settings menus managed by a stack system.
    Only one UI element visible at a time.
    Basic implementation with a few navigable buttons.

12. Asset Loader Utility
    Efficiently loads assets on demand instead of holding everything in memory.

13. Transformation Ability
    Ability to transform into another character for faster world traversal.
    Conceptual implementation with more polish needed.

14. Communication Interfaces
    Interfaces to facilitate communication between various scripts.

15. Component-Based Architecture
    Combat Component: Handles gun logic and combat mechanics.
    Montages Component: Manages player animation montages like firing and reloading.
    Attribute Component: Standalone component for health and related attributes.
    Action Component: Manages magic abilities and their logic.
    Player Inventory: Basic inventory system for item management.

16. Player Inventory System
    Pick up, drop, and stack items seamlessly.
    Two item base classes currently exist: one for healing and buffs, and another for inventory items. These may be merged or renamed for clarity.

17. Enums and Structs
    Various enums and structs for easier data handling and creating data tables.
