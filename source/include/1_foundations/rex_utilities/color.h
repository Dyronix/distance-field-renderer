#pragma once

#include <ostream>
#include <array>

// Helpers macros to generate 32-bit encoded colors
#ifdef IMGUI_USE_BGRA_PACKED_COLOR
#define REX_COLOR_32_R_SHIFT 16
#define REX_COLOR_32_G_SHIFT 8
#define REX_COLOR_32_B_SHIFT 0
#define REX_COLOR_32_A_SHIFT 24
#define REX_COLOR_32_A_MASK 0xFF000000
#else
#define REX_COLOR_32_R_SHIFT 0
#define REX_COLOR_32_G_SHIFT 8
#define REX_COLOR_32_B_SHIFT 16
#define REX_COLOR_32_A_SHIFT 24
#define REX_COLOR_32_A_MASK 0xFF000000
#endif
#define REX_COLOR_32(R, G, B, A) (((uint32)(A) << REX_COLOR_32_A_SHIFT) | ((uint32)(B) << REX_COLOR_32_B_SHIFT) | ((uint32)(G) << REX_COLOR_32_G_SHIFT) | ((uint32)(R) << REX_COLOR_32_R_SHIFT))
#define REX_COLOR_32_WHITE REX_COLOR_32(255, 255, 255, 255)  // Opaque white = 0xFFFFFFFF
#define REX_COLOR_32_BLACK REX_COLOR_32(0, 0, 0, 255)        // Opaque black
#define REX_COLOR_32_BLACK_TRANS REX_COLOR_32(0, 0, 0, 0)    // Transparent black = 0x00000000

namespace rex
{
    struct Color3
    {
        //-------------------------------------------------------------------------
        constexpr Color3()
            : red(0.0f)
            , green(0.0f)
            , blue(0.0f)
        {
        }

        //-------------------------------------------------------------------------
        constexpr Color3(float r, float g, float b)
            : red(r)
            , green(g)
            , blue(b)
        {
        }

        //-------------------------------------------------------------------------
        float* get_data()
        {
            return color_data.data();
        }
        //-------------------------------------------------------------------------
        const float* get_data() const
        {
            return color_data.data();
        }

        union
        {
// Disabling this warning because it has to be nameless.
#pragma warning(push)
#pragma warning(disable : 4201)
            struct
            {
                float red, green, blue;
            };
#pragma warning(pop)
            std::array<float, 3> color_data;
        };
    };

    struct Color4
    {
        //-------------------------------------------------------------------------
        constexpr Color4()
            : red(0.0f)
            , green(0.0f)
            , blue(0.0f)
            , alpha(1.0f)
        {
        }

        //-------------------------------------------------------------------------
        constexpr Color4(float r, float g, float b, float a = 1.0f)
            : red(r)
            , green(g)
            , blue(b)
            , alpha(a)
        {
        }

        //-------------------------------------------------------------------------
        float* get_data()
        {
            return color_data.data();
        }
        //-------------------------------------------------------------------------
        const float* get_data() const
        {
            return color_data.data();
        }

        union
        {
// Disabling this warning because it has to be nameless.
#pragma warning(push)
#pragma warning(disable : 4201)
            struct
            {
                float red, green, blue, alpha;
            };
#pragma warning(pop)
            std::array<float, 4> color_data;
        };
    };

    //-------------------------------------------------------------------------
    inline std::ostream& operator<<(std::ostream& os, const Color3& e)
    {
        os << "red: " << e.red << " green: " << e.green << " blue: " << e.blue;
        return os;
    }
    //-------------------------------------------------------------------------
    inline std::ostream& operator<<(std::ostream& os, const Color4& e)
    {
        os << "red: " << e.red << " green: " << e.green << " blue: " << e.blue << " alpha: " << e.alpha;
        return os;
    }

    //-------------------------------------------------------------------------
    inline Color3 to_color3(const Color4& rgba)
    {
        return Color3(rgba.red, rgba.green, rgba.blue);
    }
    //-------------------------------------------------------------------------
    inline Color4 to_color4(const Color3& rgb)
    {
        return Color4(rgb.red, rgb.green, rgb.blue, 1.0f);
    }
    //-------------------------------------------------------------------------
    inline Color4 to_color4(const Color3& rgb, float alpha)
    {
        return Color4(rgb.red, rgb.green, rgb.blue, alpha > 1.0f ? 1.0f : alpha < 0.0f ? 0.0f : alpha);
    }

    //-------------------------------------------------------------------------
    template <typename TColor>
    inline float red(const TColor& color)
    {
        return color.red;
    }
    //-------------------------------------------------------------------------
    template <typename TColor>
    inline float green(const TColor& color)
    {
        return color.green;
    }
    //-------------------------------------------------------------------------
    template <typename TColor>
    inline float blue(const TColor& color)
    {
        return color.blue;
    }
    //-------------------------------------------------------------------------
    inline float alpha(const Color4& color)
    {
        return color.alpha;
    }

    using ColorRGB = Color3;
    using ColorRGBA = Color4;

    using Color = ColorRGBA;

    namespace colors
    {
        // Standard colors (Red/Green/Blue/Alpha)
        // These colors are sRGB!
        constexpr Color4 aliceblue(0.941176534f, 0.972549081f, 1.000000000f, 1.000000000f);
        constexpr Color4 antiquewhite(0.980392218f, 0.921568692f, 0.843137324f, 1.000000000f);
        constexpr Color4 aqua(0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f);
        constexpr Color4 aquamarine(0.498039246f, 1.000000000f, 0.831372619f, 1.000000000f);
        constexpr Color4 azure(0.941176534f, 1.000000000f, 1.000000000f, 1.000000000f);
        constexpr Color4 beige(0.960784376f, 0.960784376f, 0.862745166f, 1.000000000f);
        constexpr Color4 bisque(1.000000000f, 0.894117713f, 0.768627524f, 1.000000000f);
        constexpr Color4 black(0.000000000f, 0.000000000f, 0.000000000f, 1.000000000f);
        constexpr Color4 blanchedalmond(1.000000000f, 0.921568692f, 0.803921640f, 1.000000000f);
        constexpr Color4 blue(0.000000000f, 0.000000000f, 1.000000000f, 1.000000000f);
        constexpr Color4 blueviolet(0.541176498f, 0.168627456f, 0.886274576f, 1.000000000f);
        constexpr Color4 brown(0.647058845f, 0.164705887f, 0.164705887f, 1.000000000f);
        constexpr Color4 burlywood(0.870588303f, 0.721568644f, 0.529411793f, 1.000000000f);
        constexpr Color4 cadetblue(0.372549027f, 0.619607866f, 0.627451003f, 1.000000000f);
        constexpr Color4 chartreuse(0.498039246f, 1.000000000f, 0.000000000f, 1.000000000f);
        constexpr Color4 chocolate(0.823529482f, 0.411764741f, 0.117647067f, 1.000000000f);
        constexpr Color4 coral(1.000000000f, 0.498039246f, 0.313725501f, 1.000000000f);
        constexpr Color4 cornflowerblue(0.392156899f, 0.584313750f, 0.929411829f, 1.000000000f);
        constexpr Color4 cornsilk(1.000000000f, 0.972549081f, 0.862745166f, 1.000000000f);
        constexpr Color4 crimson(0.862745166f, 0.078431375f, 0.235294133f, 1.000000000f);
        constexpr Color4 cyan(0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f);
        constexpr Color4 darkblue(0.000000000f, 0.000000000f, 0.545098066f, 1.000000000f);
        constexpr Color4 darkcyan(0.000000000f, 0.545098066f, 0.545098066f, 1.000000000f);
        constexpr Color4 darkgoldenrod(0.721568644f, 0.525490224f, 0.043137256f, 1.000000000f);
        constexpr Color4 darkgray(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f);
        constexpr Color4 darkgreen(0.000000000f, 0.392156899f, 0.000000000f, 1.000000000f);
        constexpr Color4 darkkhaki(0.741176486f, 0.717647076f, 0.419607878f, 1.000000000f);
        constexpr Color4 darkmagenta(0.545098066f, 0.000000000f, 0.545098066f, 1.000000000f);
        constexpr Color4 darkolivegreen(0.333333343f, 0.419607878f, 0.184313729f, 1.000000000f);
        constexpr Color4 darkorange(1.000000000f, 0.549019635f, 0.000000000f, 1.000000000f);
        constexpr Color4 darkorchid(0.600000024f, 0.196078449f, 0.800000072f, 1.000000000f);
        constexpr Color4 darkred(0.545098066f, 0.000000000f, 0.000000000f, 1.000000000f);
        constexpr Color4 darksalmon(0.913725555f, 0.588235319f, 0.478431404f, 1.000000000f);
        constexpr Color4 darkseagreen(0.560784340f, 0.737254918f, 0.545098066f, 1.000000000f);
        constexpr Color4 darkslateblue(0.282352954f, 0.239215702f, 0.545098066f, 1.000000000f);
        constexpr Color4 darkslategray(0.184313729f, 0.309803933f, 0.309803933f, 1.000000000f);
        constexpr Color4 darkturquoise(0.000000000f, 0.807843208f, 0.819607913f, 1.000000000f);
        constexpr Color4 darkviolet(0.580392182f, 0.000000000f, 0.827451050f, 1.000000000f);
        constexpr Color4 deeppink(1.000000000f, 0.078431375f, 0.576470613f, 1.000000000f);
        constexpr Color4 deepskyblue(0.000000000f, 0.749019623f, 1.000000000f, 1.000000000f);
        constexpr Color4 dimgray(0.411764741f, 0.411764741f, 0.411764741f, 1.000000000f);
        constexpr Color4 dodgerblue(0.117647067f, 0.564705908f, 1.000000000f, 1.000000000f);
        constexpr Color4 firebrick(0.698039234f, 0.133333340f, 0.133333340f, 1.000000000f);
        constexpr Color4 floralwhite(1.000000000f, 0.980392218f, 0.941176534f, 1.000000000f);
        constexpr Color4 forestgreen(0.133333340f, 0.545098066f, 0.133333340f, 1.000000000f);
        constexpr Color4 fuchsia(1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f);
        constexpr Color4 gainsboro(0.862745166f, 0.862745166f, 0.862745166f, 1.000000000f);
        constexpr Color4 ghostwhite(0.972549081f, 0.972549081f, 1.000000000f, 1.000000000f);
        constexpr Color4 gold(1.000000000f, 0.843137324f, 0.000000000f, 1.000000000f);
        constexpr Color4 goldenrod(0.854902029f, 0.647058845f, 0.125490203f, 1.000000000f);
        constexpr Color4 gray(0.501960814f, 0.501960814f, 0.501960814f, 1.000000000f);
        constexpr Color4 green(0.000000000f, 0.501960814f, 0.000000000f, 1.000000000f);
        constexpr Color4 greenyellow(0.678431392f, 1.000000000f, 0.184313729f, 1.000000000f);
        constexpr Color4 honeydew(0.941176534f, 1.000000000f, 0.941176534f, 1.000000000f);
        constexpr Color4 hotpink(1.000000000f, 0.411764741f, 0.705882370f, 1.000000000f);
        constexpr Color4 indianred(0.803921640f, 0.360784322f, 0.360784322f, 1.000000000f);
        constexpr Color4 indigo(0.294117659f, 0.000000000f, 0.509803951f, 1.000000000f);
        constexpr Color4 ivory(1.000000000f, 1.000000000f, 0.941176534f, 1.000000000f);
        constexpr Color4 khaki(0.941176534f, 0.901960850f, 0.549019635f, 1.000000000f);
        constexpr Color4 lavender(0.901960850f, 0.901960850f, 0.980392218f, 1.000000000f);
        constexpr Color4 lavenderblush(1.000000000f, 0.941176534f, 0.960784376f, 1.000000000f);
        constexpr Color4 lawngreen(0.486274540f, 0.988235354f, 0.000000000f, 1.000000000f);
        constexpr Color4 lemonchiffon(1.000000000f, 0.980392218f, 0.803921640f, 1.000000000f);
        constexpr Color4 lightblue(0.678431392f, 0.847058892f, 0.901960850f, 1.000000000f);
        constexpr Color4 lightcoral(0.941176534f, 0.501960814f, 0.501960814f, 1.000000000f);
        constexpr Color4 lightcyan(0.878431439f, 1.000000000f, 1.000000000f, 1.000000000f);
        constexpr Color4 lightgoldenrodyellow(0.980392218f, 0.980392218f, 0.823529482f, 1.000000000f);
        constexpr Color4 lightgreen(0.564705908f, 0.933333397f, 0.564705908f, 1.000000000f);
        constexpr Color4 lightgray(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f);
        constexpr Color4 lightpink(1.000000000f, 0.713725507f, 0.756862819f, 1.000000000f);
        constexpr Color4 lightsalmon(1.000000000f, 0.627451003f, 0.478431404f, 1.000000000f);
        constexpr Color4 lightseagreen(0.125490203f, 0.698039234f, 0.666666687f, 1.000000000f);
        constexpr Color4 lightskyblue(0.529411793f, 0.807843208f, 0.980392218f, 1.000000000f);
        constexpr Color4 lightslategray(0.466666698f, 0.533333361f, 0.600000024f, 1.000000000f);
        constexpr Color4 lightsteelblue(0.690196097f, 0.768627524f, 0.870588303f, 1.000000000f);
        constexpr Color4 lightyellow(1.000000000f, 1.000000000f, 0.878431439f, 1.000000000f);
        constexpr Color4 lime(0.000000000f, 1.000000000f, 0.000000000f, 1.000000000f);
        constexpr Color4 limegreen(0.196078449f, 0.803921640f, 0.196078449f, 1.000000000f);
        constexpr Color4 linen(0.980392218f, 0.941176534f, 0.901960850f, 1.000000000f);
        constexpr Color4 magenta(1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f);
        constexpr Color4 maroon(0.501960814f, 0.000000000f, 0.000000000f, 1.000000000f);
        constexpr Color4 mediumaquamarine(0.400000036f, 0.803921640f, 0.666666687f, 1.000000000f);
        constexpr Color4 mediumblue(0.000000000f, 0.000000000f, 0.803921640f, 1.000000000f);
        constexpr Color4 mediumorchid(0.729411781f, 0.333333343f, 0.827451050f, 1.000000000f);
        constexpr Color4 mediumpurple(0.576470613f, 0.439215720f, 0.858823597f, 1.000000000f);
        constexpr Color4 mediumseagreen(0.235294133f, 0.701960802f, 0.443137288f, 1.000000000f);
        constexpr Color4 mediumslateblue(0.482352972f, 0.407843173f, 0.933333397f, 1.000000000f);
        constexpr Color4 mediumspringgreen(0.000000000f, 0.980392218f, 0.603921592f, 1.000000000f);
        constexpr Color4 mediumturquoise(0.282352954f, 0.819607913f, 0.800000072f, 1.000000000f);
        constexpr Color4 mediumvioletred(0.780392230f, 0.082352944f, 0.521568656f, 1.000000000f);
        constexpr Color4 midnightblue(0.098039225f, 0.098039225f, 0.439215720f, 1.000000000f);
        constexpr Color4 mintcream(0.960784376f, 1.000000000f, 0.980392218f, 1.000000000f);
        constexpr Color4 mistyrose(1.000000000f, 0.894117713f, 0.882353008f, 1.000000000f);
        constexpr Color4 moccasin(1.000000000f, 0.894117713f, 0.709803939f, 1.000000000f);
        constexpr Color4 navajowhite(1.000000000f, 0.870588303f, 0.678431392f, 1.000000000f);
        constexpr Color4 navy(0.000000000f, 0.000000000f, 0.501960814f, 1.000000000f);
        constexpr Color4 oldlace(0.992156923f, 0.960784376f, 0.901960850f, 1.000000000f);
        constexpr Color4 olive(0.501960814f, 0.501960814f, 0.000000000f, 1.000000000f);
        constexpr Color4 olivedrab(0.419607878f, 0.556862772f, 0.137254909f, 1.000000000f);
        constexpr Color4 orange(1.000000000f, 0.647058845f, 0.000000000f, 1.000000000f);
        constexpr Color4 orangered(1.000000000f, 0.270588249f, 0.000000000f, 1.000000000f);
        constexpr Color4 orchid(0.854902029f, 0.439215720f, 0.839215755f, 1.000000000f);
        constexpr Color4 palegoldenrod(0.933333397f, 0.909803987f, 0.666666687f, 1.000000000f);
        constexpr Color4 palegreen(0.596078455f, 0.984313786f, 0.596078455f, 1.000000000f);
        constexpr Color4 paleturquoise(0.686274529f, 0.933333397f, 0.933333397f, 1.000000000f);
        constexpr Color4 palevioletred(0.858823597f, 0.439215720f, 0.576470613f, 1.000000000f);
        constexpr Color4 papayawhip(1.000000000f, 0.937254965f, 0.835294187f, 1.000000000f);
        constexpr Color4 peachpuff(1.000000000f, 0.854902029f, 0.725490212f, 1.000000000f);
        constexpr Color4 peru(0.803921640f, 0.521568656f, 0.247058839f, 1.000000000f);
        constexpr Color4 pink(1.000000000f, 0.752941251f, 0.796078503f, 1.000000000f);
        constexpr Color4 plum(0.866666734f, 0.627451003f, 0.866666734f, 1.000000000f);
        constexpr Color4 powderblue(0.690196097f, 0.878431439f, 0.901960850f, 1.000000000f);
        constexpr Color4 purple(0.501960814f, 0.000000000f, 0.501960814f, 1.000000000f);
        constexpr Color4 red(1.000000000f, 0.000000000f, 0.000000000f, 1.000000000f);
        constexpr Color4 rosybrown(0.737254918f, 0.560784340f, 0.560784340f, 1.000000000f);
        constexpr Color4 royalblue(0.254901975f, 0.411764741f, 0.882353008f, 1.000000000f);
        constexpr Color4 saddlebrown(0.545098066f, 0.270588249f, 0.074509807f, 1.000000000f);
        constexpr Color4 salmon(0.980392218f, 0.501960814f, 0.447058856f, 1.000000000f);
        constexpr Color4 sandybrown(0.956862807f, 0.643137276f, 0.376470625f, 1.000000000f);
        constexpr Color4 seagreen(0.180392161f, 0.545098066f, 0.341176480f, 1.000000000f);
        constexpr Color4 seashell(1.000000000f, 0.960784376f, 0.933333397f, 1.000000000f);
        constexpr Color4 sienna(0.627451003f, 0.321568638f, 0.176470593f, 1.000000000f);
        constexpr Color4 silver(0.752941251f, 0.752941251f, 0.752941251f, 1.000000000f);
        constexpr Color4 skyblue(0.529411793f, 0.807843208f, 0.921568692f, 1.000000000f);
        constexpr Color4 slateblue(0.415686309f, 0.352941185f, 0.803921640f, 1.000000000f);
        constexpr Color4 slategray(0.439215720f, 0.501960814f, 0.564705908f, 1.000000000f);
        constexpr Color4 snow(1.000000000f, 0.980392218f, 0.980392218f, 1.000000000f);
        constexpr Color4 springgreen(0.000000000f, 1.000000000f, 0.498039246f, 1.000000000f);
        constexpr Color4 steelblue(0.274509817f, 0.509803951f, 0.705882370f, 1.000000000f);
        constexpr Color4 tan(0.823529482f, 0.705882370f, 0.549019635f, 1.000000000f);
        constexpr Color4 teal(0.000000000f, 0.501960814f, 0.501960814f, 1.000000000f);
        constexpr Color4 thistle(0.847058892f, 0.749019623f, 0.847058892f, 1.000000000f);
        constexpr Color4 tomato(1.000000000f, 0.388235331f, 0.278431386f, 1.000000000f);
        constexpr Color4 transparent(0.000000000f, 0.000000000f, 0.000000000f, 0.000000000f);
        constexpr Color4 turquoise(0.250980407f, 0.878431439f, 0.815686345f, 1.000000000f);
        constexpr Color4 violet(0.933333397f, 0.509803951f, 0.933333397f, 1.000000000f);
        constexpr Color4 wheat(0.960784376f, 0.870588303f, 0.701960802f, 1.000000000f);
        constexpr Color4 white(1.000000000f, 1.000000000f, 1.000000000f, 1.000000000f);
        constexpr Color4 whitesmoke(0.960784376f, 0.960784376f, 0.960784376f, 1.000000000f);
        constexpr Color4 yellow(1.000000000f, 1.000000000f, 0.000000000f, 1.000000000f);
        constexpr Color4 yellowgreen(0.603921592f, 0.803921640f, 0.196078449f, 1.000000000f);
    }
}