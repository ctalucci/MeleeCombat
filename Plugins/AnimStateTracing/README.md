# Anim State Tracing Plugin

A professional animation tracing system for Unreal Engine 5 that provides bone capsule collision detection during animation sequences. Perfect for melee combat systems, weapon tracing, and precise collision detection in animated sequences.

## Features

### Core Components
- **AnimNotifyState_Tracing**: Animation notify state for capsule tracing between bone sockets
- **AnimTraceSpecComponent**: Actor component that manages trace specifications and collision handling
- **AnimTraceListenerComponent**: Component for receiving trace events and handling collision responses

### Key Capabilities
- **Bone-to-Bone Capsule Tracing**: Trace capsules between any two bone sockets during animations
- **Interpolated Tracing**: Automatic interpolation for fast animations to prevent collision gaps
- **Collision Management**: Comprehensive collision detection with customizable trace channels
- **Replication Support**: Built-in networking for multiplayer scenarios
- **Debug Visualization**: Visual debugging tools for trace development
- **Procedural Aiming**: Dynamic aim adjustment based on bone rotations
- **Hit Prevention**: Optional system to prevent multiple hits on the same actor

## Quick Start

### 1. Enable the Plugin
1. Copy the plugin to your project's `Plugins` folder
2. Enable "Anim State Tracing" in the Plugin Browser
3. Compile your project

### 2. Basic Setup

#### Add Components to Your Character
```cpp
// In your character class
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
class UAnimTraceSpecComponent* TraceSpec;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)  
class UAnimTraceListenerComponent* TraceListener;
```

#### Configure Trace Settings
- Set `CollisionTraceType` to your desired trace channel
- Configure `bShouldIgnoreHitActors` to prevent multiple hits
- Adjust `AimModifierWhileTraceActive` for dynamic aiming

### 3. Animation Setup

#### Add Tracing Notify State
1. Open your animation sequence
2. Add "Tracing" notify state to the timeline
3. Configure socket names (e.g., "hand_l" to "weapon_tip")
4. Set capsule size and interpolation settings
5. Enable debug draw for development

#### Socket Configuration
- `FirstSocketName`: Starting bone socket (e.g., hand bone)
- `SecondSocketName`: Ending bone socket (e.g., weapon tip)
- `CapsuleSize`: Radius of the trace capsule
- `MinTraceDistance`: Minimum distance before interpolation kicks in

## Advanced Usage

### Custom Trace Handling
```cpp
// Override trace events in your component
void OnTraceHitActorDelegate(AActor* ActorOwner, FName FirstSocketName, 
                           FName SecondSocketName, FHitResult TraceHit, 
                           FVector StrikeDirection, AActor* HitActor)
{
    // Handle collision logic here
    if (HitActor)
    {
        // Apply damage, effects, etc.
    }
}
```

### Procedural Aiming
The plugin supports dynamic aim adjustment:
- Configure `ProceduralAimBone` for the bone that controls aiming
- Use `AimPitchCurve` to fine-tune pitch adjustments
- Modify `AimModifierWhileTraceActive` for trace-specific aiming

### Multiplayer Support
Built-in replication functions:
- `Server_OnTraceHitActor`: Send hit data to server
- `Multicast_OnTraceHitActor`: Broadcast hit data to clients

## Configuration Options

### Trace Settings
- **Collision Trace Type**: ETraceTypeQuery for hit detection
- **Trace Ignores**: Array of actors to ignore during tracing
- **Should Ignore Hit Actors**: Prevent multiple hits on same actor
- **Enable Debug Draw**: Visual debugging for development

### Animation Settings  
- **Should Interpolate Traces**: Enable/disable trace interpolation
- **Min Trace Distance**: Distance threshold for interpolation
- **Capsule Size**: Radius of trace capsules
- **Enable Debug Draw**: Show trace capsules in viewport

## Best Practices

### Performance
- Use appropriate capsule sizes (larger = more expensive)
- Consider trace frequency vs. animation speed
- Limit debug drawing to development builds
- Use trace ignores efficiently

### Animation Design
- Place notify states precisely on contact frames
- Test with various animation speeds
- Use interpolation for fast movements
- Consider bone hierarchy when choosing sockets

### Collision Setup
- Create dedicated collision channels for tracing
- Set up collision responses properly
- Test with different object types
- Consider using collision presets

## Troubleshooting

### Common Issues
- **No traces appearing**: Check socket names and bone hierarchy
- **Poor performance**: Reduce capsule size or trace frequency  
- **Missing hits**: Enable interpolation or reduce MinTraceDistance
- **Multiple hits**: Enable bShouldIgnoreHitActors

### Debug Tools
- Enable debug drawing in both component and notify state
- Use animation blueprint debugging
- Check collision channel settings
- Verify socket positions in skeleton viewer

## Technical Requirements

- **Engine Version**: Unreal Engine 5.0+
- **Platforms**: Windows, Mac, Linux, Android
- **Module Type**: Runtime
- **Dependencies**: Core animation and collision systems

## License

Copyright Chris Talucci. All rights reserved.

## Support

For technical support and feature requests, please contact the developer through the Unreal Engine Marketplace.

---

*This plugin is designed for professional game development and provides robust animation-based collision detection for combat systems, interactive objects, and precise gameplay mechanics.*