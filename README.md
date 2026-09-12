# About
Here is a list of some of the features of this plugin:
- Console commands helpers to easily make console vars, commands, ... (See `FUConsole.h`) such as:
   - `FU_CMD_AUTOVAR`, `FU_CMD_AUTOVAR_EXTERN`, `FU_CMD_RUNFUNC_ACTOR_SINGLE`, `FU_CMD_RUNFUNC_OBJECT_SINGLE`, `FU_CMD_RUNLAMBDA`
- Logging helpers (See `FULogging.h`)
   - `_FU_LOG_OBJECT` (`_FU_LOG_OBJECT_D`, `_FU_LOG_OBJECT_W`, `_FU_LOG_OBJECT_E`, `_FU_LOG_OBJECT_S`), `_FU_LOG_STATIC` (`_FU_LOG_STATIC_D`, `_FU_LOG_STATIC_W`, `_FU_LOG_STATIC_E`, `_FU_LOG_STATIC_S`)
- Debug draw helpers (See `FUDraw.h`) and Visual Loger helpers (See `FUVisualLoggerMacros.h`)
   - Sphere, Circle, Box, Capsule, Line, Point, Plane, String/Text, DirectionalArrow, LineWithMiddleText, DirectionalArrowWithMiddleText, ActorBounds, BodyInstance, PrimitiveComponent, Rectangle, Polygon.
   - Note: For `FFUOrientedBox` the draw functions are on the type itself
- Assert helpers (See `FUAsserts.h`) such as:
   - `FU_ENSURE`, `FU_ENSURE_MSG`, `FU_ENSURE_VALID`, `FU_ENSURE_WEAKVALID`, `FU_ENSURE_WEAKNOTNULL`
- Tick World Subsystem to register custom callbacks to various tick phases (See `UFUExtraTickWorldSubsystem`)
- In editor additions:
   - Select actors in current focused folder
   - Copy/Paste Transform/Location/Rotation/Scale submenu on actor selection
   - Mirror actor preview on actor selection
- Various misc console commands (and functions!) helpers to debug performance issues and more
   - Debug Primitive Components using overlapping without reason for all actors in a world (`FU.Editor.PrintAllGenerateOverlapEventsComponentsFromCurrentWorld`), all actor assets in the whole project (`FU.Editor.PrintAllGenerateOverlapEventsComponentsFromAssets`) or at a given path (`FU.Editor.PrintAllGenerateOverlapEventsComponentsFromPath`)
   - Equivalent but for CollisionEnabled (`FU.Editor.PrintAllCollisionEnabledComponentsFromWorld`)
- Misc helper types and functions such as:
   - `FFUMessageBuilder` to ease the generation of a single string from various sources.
   - `FFUOrientedBox` to generate and draw an oriented box from an actor or primitive components
   - `FFUEditorValidatorContainerBase` for misc editor validation
   - `UFUUILibrary`, `UFUUtilityLibrary` for misc static helpers

# How to add this plugin to your project
1. Go at your project location folder (where the `.uproject` file is)
2. Go in the `Plugins` folder (create it if necessary)
3. In the windows cmd (or anything else you might use) clone this repo and the `BlueprintComponentReferencePlugin` dependency ([Check plugin](https://github.com/aquanox/BlueprintComponentReferencePlugin))
  - `git clone https://github.com/hzFishy/FishyUtils.git`
  - `git clone https://github.com/hzFishy/BCRP.git` ([See fork](https://github.com/hzFishy/BCRP))
4. Now you can include `FishyUtils` in your project (or any other plugin) and use it.

If you really don't want to add `BlueprintComponentReferencePlugin` (or BCRP) to your project, you can still clone this repo only and manually edit a few files (and remove some of them) to avoid the dependency. Only some features of FishyUtils uses the plugin.
