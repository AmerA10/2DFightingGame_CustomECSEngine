-- we want to load a different tilemap image depending on system time
local current_system_hour = os.date("*t").hour

local map_texture_asset_id

if current_system_hour >= 9 and current_system_hour < 18 then
    map_texture_asset_id = "tilemap-texture-night"
else
    map_texture_asset_id = "tilemap-texture-day"
end

-- Define a table with the values of the first level
Level = {
    ----------------------------------------------------
    -- Table to define the list of assets
    ----------------------------------------------------
    assets = {
        [0] =
        { type = "texture", id = "tilemap-texture-day", file = "./assets/tilemaps/jungle.png" },
        { type = "texture", id = "tilemap-texture-night", file = "./assets/tilemaps/jungle-night.png" },
        { type = "texture", id = "chopper-texture", file = "./assets/images/chopper-spritesheet.png"},
        { type = "texture", id = "tank-texture",    file = "./assets/images/tank-tiger-up.png" },
        { type = "texture", id = "truck-texture",    file = "./assets/images/truck-ford-up.png" },
        { type = "texture", id = "bullet-texture",  file = "./assets/images/bullet.png" },
        { type = "texture", id = "fw190-texture",  file = "./assets/images/fw190-spritesheet.png" },
        { type = "texture", id = "ryu-idle-texture",  file = "./assets/images/ruy_idle_done.png" },
        { type = "texture", id = "ryu-hk-texture",  file = "./assets/images/ryu_hk_done.png" },
        { type = "font"   , id = "arial-font",    file = "./assets/fonts/arial.ttf", font_size = 20 },
        { type = "font"   , id = "charriot-font",   file = "./assets/fonts/charriot.ttf", font_size = 20 },
        { type = "sound"  , id = "helicopter-sound", file= "./assets/sounds/helicopter.wav"},
        { type = "sound"  , id = "laser-sound",    file="./assets/sounds/laser.ogg"},
        { type = "song",    id = "level1-music",   file="./assets/sounds/level1music.mp3" },
        {type = "sprite_sheet", id = "fighter_1_hk_sheet", file = "./assets/images/ryu_hk_done.png", num_frames = 6, 
        
            frame_data = 
            {
                [0] = 
                { x = 276, y = 0,  w = 57, h = 117},
                { x = 215, y = 0,  w = 61, h = 117},
                { x = 0,   y = 0,  w = 114,h = 117},
                { x = 114, y = 0,  w = 101,h = 117},
                { x = 333, y = 0,w = 55, h = 117},
                { x = 388, y = 0,  w = 54, h = 117},
            }
        },
        { type = "animClip",id = "chopper-idle", duration = 2.0, sprite_sheet_id = "chopper-texture", 
            
            timeToFrames = 
            {
                [0] = 
                {t = 0.5, f = 0, 
                
                    e = function(entity) 
                            set_position(entity,math.random(10,100),math.random(100,200)) 
                        end
                }, 
                {t = 1.0, f = 1},
                {t = 1.25, e = function(entity)
                                set_position(entity,0,0)
                            end
                },
                {t = 1.5, f = 2}
            }
        },

        {type = "fightAnimationClip", id = "fighter_1_hk", duration = 120, sprite_sheet_id = "fighter_1_hk_sheet",
        
            frameNumToFrames =
            {
                [0] =
                {t = 0,  f = 0},
                {t = 20, f = 1},
                {t = 40, f = 2},
                {t = 60, f = 3},
                {t = 80, f = 4},
                {t = 100,f = 5}
 

            }
        }


    },

    ----------------------------------------------------
    -- table to define the map config variables
    ----------------------------------------------------
    tilemap = {
        map_file = "./assets/tilemaps/jungle.map",
        texture_asset_id = map_texture_asset_id,
        num_rows = 20,
        num_cols = 25,
        tile_size = 32,
        scale = 4.0
    },

    ----------------------------------------------------
    -- table to define entities and their components
    ----------------------------------------------------
    entities = 
    {
        [0] =
        {
            -- Player
            tag = "Player",
            components = {
                transform = {
                    position = { x = math.random(10,200), y = 110 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                rigidbody = {
                    velocity = { x = 0.0, y = 0.0 }
                },
                sprite = {
                    texture_asset_id = "fighter_1_hk_sheet",
                    width = 100,
                    height = 100,
                    z_index = 4,
                    fixed = false,
                    src_rect_x = 0,
                    src_rect_y = 0
                },
                f_animation = {
                    clip_id = "fighter_1_hk",
                    playback_rate = 1
                },
                boxcollider = {
                    width = 76,
                    height = 100,
                    scale = { x = 2.0, y = 2.0},
                    offset = { x = 0, y = 0 },
                    layer = "10000000000000000000000000000000",
                    mask =  "00000000000000000000000000000000"
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 200, y = 200 },
                    projectile_duration = 10, -- seconds
                    repeat_frequency = 0, -- seconds
                    hit_percentage_damage = 10,
                    friendly = true,
                    projectile_speed = 200,
                    use_frequency = false

                },
                keyboard_controller = {
                    up_velocity = { x = 0, y = -100 },
                    right_velocity = { x = 100, y = 0 },
                },
                camera_follow = {
                    follow = true
                },
                text_label = {
                    font_asset_id = "charriot-font",
                    text = "",
                    text_position = { x = 242,  y = 110 },
                    color = { r = 50, g = 50, b = 50},
                    is_fixed = false
                },
                sound = {
                    --placeHolder
                }
            }
        },
        {
            -- Tank
            group = "Enemies",
            components = {
                transform = {
                    position = { x = 200, y = 497 },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-texture",
                    width = 32,
                    height = 32,
                    z_index = 2
                },
                boxcollider = {
                    width = 32,
                    height = 32,
                    scale = { x = 2.0, y = 2.0},
                    offset = { x = 0, y = 0 },
                    layer = "01000000000000000000000000000000",
                    mask =  "00000000000000000000000000000000"
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 100, y = 0 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 20,
                    friendly = false,
                    projectile_speed = 200,
                    use_frequency = true
                },
                text_label = {
                    font_asset_id = "charriot-font",
                    text = "",
                    color = { r = 150, g = 250, b = 50},
                    is_fixed = false
                }
            } 
        },
        {
            -- Fw190
            group = "Enemies",
            components = {
                transform = {
                    position = { x = math.random(200,300), y = math.random(200,500) },
                    scale = { x = 2.0, y = 2.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "fw190-texture",
                    width = 32,
                    height = 32,
                    z_index = 2
                },
                boxcollider = {
                    width = 32,
                    height = 32,
                    scale = { x = 2.0, y = 2.0},
                    offset = { x = 0, y = 0 },
                    layer = "01000000000000000000000000000000",
                    mask =  "00000000000000000000000000000000"
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 100, y = 0 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 20,
                    friendly = false,
                    projectile_speed = 200,
                    use_frequency = true
                },
                text_label = {
                    font_asset_id = "charriot-font",
                    text = "",
                    color = { r = 150, g = 250, b = 50},
                    is_fixed = false
                },
                on_update_script = {
                    [0] = 
                    function (entity, delta_time, ellapsed_time)
                        local position_x, position_y = get_position(entity)
                        local new_x = ellapsed_time * 0.09
                        local new_y = 200 + (math.sin(ellapsed_time * 0.001) * 50)
                        set_position(entity,new_x,new_y)

                    end
                },
            }
        },
        {
            --MusicPlayer
            tag = "MusicPlayer",
            components =
            {
                sound=
                {
                    sound_asset_id="level1-music",
                    sound_loop=true,
                    is_music=true,
                    sound_volume = 50
                }
            }
        }
    }
}

map_width = Level.tilemap.tile_size * Level.tilemap.num_cols * Level.tilemap.scale
map_height = Level.tilemap.tile_size * Level.tilemap.num_rows * Level.tilemap.scale