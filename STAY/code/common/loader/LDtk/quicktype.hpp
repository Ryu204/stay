// NOLINTBEGIN
//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Coordinate data = nlohmann::json::parse(jsonString);

#pragma once

#include <optional>

#include "nlohmann/json.hpp"

#include <optional>
#include <stdexcept>
#include <regex>

#ifndef NLOHMANN_OPT_HELPER
#define NLOHMANN_OPT_HELPER
namespace nlohmann {
    template <typename T>
    struct adl_serializer<std::shared_ptr<T>> {
        static void to_json(json & j, const std::shared_ptr<T> & opt) {
            if (!opt) j = nullptr; else j = *opt;
        }

        static std::shared_ptr<T> from_json(const json & j) {
            if (j.is_null()) return std::make_shared<T>(); else return std::make_shared<T>(j.get<T>());
        }
    };
    template <typename T>
    struct adl_serializer<std::optional<T>> {
        static void to_json(json & j, const std::optional<T> & opt) {
            if (!opt) j = nullptr; else j = *opt;
        }

        static std::optional<T> from_json(const json & j) {
            if (j.is_null()) return std::make_optional<T>(); else return std::make_optional<T>(j.get<T>());
        }
    };
}
#endif

namespace ldtk {
    using nlohmann::json;

    #ifndef NLOHMANN_UNTYPED_ldtk_HELPER
    #define NLOHMANN_UNTYPED_ldtk_HELPER
    inline json get_untyped(const json & j, const char * property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json & j, std::string property) {
        return get_untyped(j, property.data());
    }
    #endif

    #ifndef NLOHMANN_OPTIONAL_ldtk_HELPER
    #define NLOHMANN_OPTIONAL_ldtk_HELPER
    template <typename T>
    inline std::shared_ptr<T> get_heap_optional(const json & j, const char * property) {
        auto it = j.find(property);
        if (it != j.end() && !it->is_null()) {
            return j.at(property).get<std::shared_ptr<T>>();
        }
        return std::shared_ptr<T>();
    }

    template <typename T>
    inline std::shared_ptr<T> get_heap_optional(const json & j, std::string property) {
        return get_heap_optional<T>(j, property.data());
    }
    template <typename T>
    inline std::optional<T> get_stack_optional(const json & j, const char * property) {
        auto it = j.find(property);
        if (it != j.end() && !it->is_null()) {
            return j.at(property).get<std::optional<T>>();
        }
        return std::optional<T>();
    }

    template <typename T>
    inline std::optional<T> get_stack_optional(const json & j, std::string property) {
        return get_stack_optional<T>(j, property.data());
    }
    #endif

    /**
     * Possible values: `Manual`, `AfterLoad`, `BeforeSave`, `AfterSave`
     */
    enum class When : int { afterLoad, afterSave, beforeSave, manual };

    class LdtkCustomCommand {
        public:
        LdtkCustomCommand() = default;
        virtual ~LdtkCustomCommand() = default;

        private:
        std::string command;
        When when;

        public:
        const std::string & getCommand() const { return command; }
        std::string & getMutableCommand() { return command; }
        void setCommand(const std::string & value) { this->command = value; }

        /**
         * Possible values: `Manual`, `AfterLoad`, `BeforeSave`, `AfterSave`
         */
        const When & getWhen() const { return when; }
        When & getMutableWhen() { return when; }
        void setWhen(const When & value) { this->when = value; }
    };

    /**
     * Possible values: `Any`, `OnlySame`, `OnlyTags`, `OnlySpecificEntity`
     */
    enum class AllowedRefs : int { any, onlySame, onlySpecificEntity, onlyTags };

    /**
     * Possible values: `Hidden`, `ValueOnly`, `NameAndValue`, `EntityTile`, `LevelTile`,
     * `Points`, `PointStar`, `PointPath`, `PointPathLoop`, `RadiusPx`, `RadiusGrid`,
     * `ArrayCountWithLabel`, `ArrayCountNoLabel`, `RefLinkBetweenPivots`,
     * `RefLinkBetweenCenters`
     */
    enum class EditorDisplayMode : int { arrayCountNoLabel, arrayCountWithLabel, entityTile, hidden, levelTile, nameAndValue, pointPath, pointPathLoop, pointStar, points, radiusGrid, radiusPx, refLinkBetweenCenters, refLinkBetweenPivots, valueOnly };

    /**
     * Possible values: `Above`, `Center`, `Beneath`
     */
    enum class EditorDisplayPos : int { above, beneath, center };

    /**
     * Possible values: `ZigZag`, `StraightArrow`, `CurvedArrow`, `ArrowsLine`, `DashedLine`
     */
    enum class EditorLinkStyle : int { arrowsLine, curvedArrow, dashedLine, straightArrow, zigZag };

    enum class TextLanguageMode : int { langC, langHaxe, langJs, langJson, langLog, langLua, langMarkdown, langPython, langRuby, langXml };

    /**
     * This section is mostly only intended for the LDtk editor app itself. You can safely
     * ignore it.
     */
    class FieldDefinition {
        public:
        FieldDefinition() = default;
        virtual ~FieldDefinition() = default;

        private:
        std::string type;
        std::optional<std::vector<std::string>> acceptFileTypes;
        AllowedRefs allowedRefs;
        std::optional<int64_t> allowedRefsEntityUid;
        std::vector<std::string> allowedRefTags;
        bool allowOutOfLevelRef;
        std::optional<int64_t> arrayMaxLength;
        std::optional<int64_t> arrayMinLength;
        bool autoChainRef;
        bool canBeNull;
        nlohmann::json defaultOverride;
        std::optional<std::string> doc;
        bool editorAlwaysShow;
        bool editorCutLongValues;
        std::optional<std::string> editorDisplayColor;
        EditorDisplayMode editorDisplayMode;
        EditorDisplayPos editorDisplayPos;
        double editorDisplayScale;
        EditorLinkStyle editorLinkStyle;
        bool editorShowInWorld;
        std::optional<std::string> editorTextPrefix;
        std::optional<std::string> editorTextSuffix;
        std::string identifier;
        bool isArray;
        std::optional<double> max;
        std::optional<double> min;
        std::optional<std::string> regex;
        bool symmetricalRef;
        std::optional<TextLanguageMode> textLanguageMode;
        std::optional<int64_t> tilesetUid;
        std::string fieldDefinitionType;
        int64_t uid;
        bool useForSmartColor;

        public:
        /**
         * Human readable value type. Possible values: `Int, Float, String, Bool, Color,
         * ExternEnum.XXX, LocalEnum.XXX, Point, FilePath`.<br/>  If the field is an array, this
         * field will look like `Array<...>` (eg. `Array<Int>`, `Array<Point>` etc.)<br/>  NOTE: if
         * you enable the advanced option **Use Multilines type**, you will have "*Multilines*"
         * instead of "*String*" when relevant.
         */
        const std::string & getType() const { return type; }
        std::string & getMutableType() { return type; }
        void setType(const std::string & value) { this->type = value; }

        /**
         * Optional list of accepted file extensions for FilePath value type. Includes the dot:
         * `.ext`
         */
        std::optional<std::vector<std::string>> getAcceptFileTypes() const { return acceptFileTypes; }
        void setAcceptFileTypes(std::optional<std::vector<std::string>> value) { this->acceptFileTypes = value; }

        /**
         * Possible values: `Any`, `OnlySame`, `OnlyTags`, `OnlySpecificEntity`
         */
        const AllowedRefs & getAllowedRefs() const { return allowedRefs; }
        AllowedRefs & getMutableAllowedRefs() { return allowedRefs; }
        void setAllowedRefs(const AllowedRefs & value) { this->allowedRefs = value; }

        std::optional<int64_t> getAllowedRefsEntityUid() const { return allowedRefsEntityUid; }
        void setAllowedRefsEntityUid(std::optional<int64_t> value) { this->allowedRefsEntityUid = value; }

        const std::vector<std::string> & getAllowedRefTags() const { return allowedRefTags; }
        std::vector<std::string> & getMutableAllowedRefTags() { return allowedRefTags; }
        void setAllowedRefTags(const std::vector<std::string> & value) { this->allowedRefTags = value; }

        const bool & getAllowOutOfLevelRef() const { return allowOutOfLevelRef; }
        bool & getMutableAllowOutOfLevelRef() { return allowOutOfLevelRef; }
        void setAllowOutOfLevelRef(const bool & value) { this->allowOutOfLevelRef = value; }

        /**
         * Array max length
         */
        std::optional<int64_t> getArrayMaxLength() const { return arrayMaxLength; }
        void setArrayMaxLength(std::optional<int64_t> value) { this->arrayMaxLength = value; }

        /**
         * Array min length
         */
        std::optional<int64_t> getArrayMinLength() const { return arrayMinLength; }
        void setArrayMinLength(std::optional<int64_t> value) { this->arrayMinLength = value; }

        const bool & getAutoChainRef() const { return autoChainRef; }
        bool & getMutableAutoChainRef() { return autoChainRef; }
        void setAutoChainRef(const bool & value) { this->autoChainRef = value; }

        /**
         * TRUE if the value can be null. For arrays, TRUE means it can contain null values
         * (exception: array of Points can't have null values).
         */
        const bool & getCanBeNull() const { return canBeNull; }
        bool & getMutableCanBeNull() { return canBeNull; }
        void setCanBeNull(const bool & value) { this->canBeNull = value; }

        /**
         * Default value if selected value is null or invalid.
         */
        const nlohmann::json & getDefaultOverride() const { return defaultOverride; }
        nlohmann::json & getMutableDefaultOverride() { return defaultOverride; }
        void setDefaultOverride(const nlohmann::json & value) { this->defaultOverride = value; }

        /**
         * User defined documentation for this field to provide help/tips to level designers about
         * accepted values.
         */
        std::optional<std::string> getDoc() const { return doc; }
        void setDoc(std::optional<std::string> value) { this->doc = value; }

        const bool & getEditorAlwaysShow() const { return editorAlwaysShow; }
        bool & getMutableEditorAlwaysShow() { return editorAlwaysShow; }
        void setEditorAlwaysShow(const bool & value) { this->editorAlwaysShow = value; }

        const bool & getEditorCutLongValues() const { return editorCutLongValues; }
        bool & getMutableEditorCutLongValues() { return editorCutLongValues; }
        void setEditorCutLongValues(const bool & value) { this->editorCutLongValues = value; }

        std::optional<std::string> getEditorDisplayColor() const { return editorDisplayColor; }
        void setEditorDisplayColor(std::optional<std::string> value) { this->editorDisplayColor = value; }

        /**
         * Possible values: `Hidden`, `ValueOnly`, `NameAndValue`, `EntityTile`, `LevelTile`,
         * `Points`, `PointStar`, `PointPath`, `PointPathLoop`, `RadiusPx`, `RadiusGrid`,
         * `ArrayCountWithLabel`, `ArrayCountNoLabel`, `RefLinkBetweenPivots`,
         * `RefLinkBetweenCenters`
         */
        const EditorDisplayMode & getEditorDisplayMode() const { return editorDisplayMode; }
        EditorDisplayMode & getMutableEditorDisplayMode() { return editorDisplayMode; }
        void setEditorDisplayMode(const EditorDisplayMode & value) { this->editorDisplayMode = value; }

        /**
         * Possible values: `Above`, `Center`, `Beneath`
         */
        const EditorDisplayPos & getEditorDisplayPos() const { return editorDisplayPos; }
        EditorDisplayPos & getMutableEditorDisplayPos() { return editorDisplayPos; }
        void setEditorDisplayPos(const EditorDisplayPos & value) { this->editorDisplayPos = value; }

        const double & getEditorDisplayScale() const { return editorDisplayScale; }
        double & getMutableEditorDisplayScale() { return editorDisplayScale; }
        void setEditorDisplayScale(const double & value) { this->editorDisplayScale = value; }

        /**
         * Possible values: `ZigZag`, `StraightArrow`, `CurvedArrow`, `ArrowsLine`, `DashedLine`
         */
        const EditorLinkStyle & getEditorLinkStyle() const { return editorLinkStyle; }
        EditorLinkStyle & getMutableEditorLinkStyle() { return editorLinkStyle; }
        void setEditorLinkStyle(const EditorLinkStyle & value) { this->editorLinkStyle = value; }

        const bool & getEditorShowInWorld() const { return editorShowInWorld; }
        bool & getMutableEditorShowInWorld() { return editorShowInWorld; }
        void setEditorShowInWorld(const bool & value) { this->editorShowInWorld = value; }

        std::optional<std::string> getEditorTextPrefix() const { return editorTextPrefix; }
        void setEditorTextPrefix(std::optional<std::string> value) { this->editorTextPrefix = value; }

        std::optional<std::string> getEditorTextSuffix() const { return editorTextSuffix; }
        void setEditorTextSuffix(std::optional<std::string> value) { this->editorTextSuffix = value; }

        /**
         * User defined unique identifier
         */
        const std::string & getIdentifier() const { return identifier; }
        std::string & getMutableIdentifier() { return identifier; }
        void setIdentifier(const std::string & value) { this->identifier = value; }

        /**
         * TRUE if the value is an array of multiple values
         */
        const bool & getIsArray() const { return isArray; }
        bool & getMutableIsArray() { return isArray; }
        void setIsArray(const bool & value) { this->isArray = value; }

        /**
         * Max limit for value, if applicable
         */
        std::optional<double> getMax() const { return max; }
        void setMax(std::optional<double> value) { this->max = value; }

        /**
         * Min limit for value, if applicable
         */
        std::optional<double> getMin() const { return min; }
        void setMin(std::optional<double> value) { this->min = value; }

        /**
         * Optional regular expression that needs to be matched to accept values. Expected format:
         * `/some_reg_ex/g`, with optional "i" flag.
         */
        std::optional<std::string> getRegex() const { return regex; }
        void setRegex(std::optional<std::string> value) { this->regex = value; }

        const bool & getSymmetricalRef() const { return symmetricalRef; }
        bool & getMutableSymmetricalRef() { return symmetricalRef; }
        void setSymmetricalRef(const bool & value) { this->symmetricalRef = value; }

        /**
         * Possible values: &lt;`null`&gt;, `LangPython`, `LangRuby`, `LangJS`, `LangLua`, `LangC`,
         * `LangHaxe`, `LangMarkdown`, `LangJson`, `LangXml`, `LangLog`
         */
        std::optional<TextLanguageMode> getTextLanguageMode() const { return textLanguageMode; }
        void setTextLanguageMode(std::optional<TextLanguageMode> value) { this->textLanguageMode = value; }

        /**
         * UID of the tileset used for a Tile
         */
        std::optional<int64_t> getTilesetUid() const { return tilesetUid; }
        void setTilesetUid(std::optional<int64_t> value) { this->tilesetUid = value; }

        /**
         * Internal enum representing the possible field types. Possible values: F_Int, F_Float,
         * F_String, F_Text, F_Bool, F_Color, F_Enum(...), F_Point, F_Path, F_EntityRef, F_Tile
         */
        const std::string & getFieldDefinitionType() const { return fieldDefinitionType; }
        std::string & getMutableFieldDefinitionType() { return fieldDefinitionType; }
        void setFieldDefinitionType(const std::string & value) { this->fieldDefinitionType = value; }

        /**
         * Unique Int identifier
         */
        const int64_t & getUid() const { return uid; }
        int64_t & getMutableUid() { return uid; }
        void setUid(const int64_t & value) { this->uid = value; }

        /**
         * If TRUE, the color associated with this field will override the Entity or Level default
         * color in the editor UI. For Enum fields, this would be the color associated to their
         * values.
         */
        const bool & getUseForSmartColor() const { return useForSmartColor; }
        bool & getMutableUseForSmartColor() { return useForSmartColor; }
        void setUseForSmartColor(const bool & value) { this->useForSmartColor = value; }
    };

    /**
     * Possible values: `DiscardOldOnes`, `PreventAdding`, `MoveLastOne`
     */
    enum class LimitBehavior : int { discardOldOnes, moveLastOne, preventAdding };

    /**
     * If TRUE, the maxCount is a "per world" limit, if FALSE, it's a "per level". Possible
     * values: `PerLayer`, `PerLevel`, `PerWorld`
     */
    enum class LimitScope : int { perLayer, perLevel, perWorld };

    /**
     * Possible values: `Rectangle`, `Ellipse`, `Tile`, `Cross`
     */
    enum class RenderMode : int { cross, ellipse, rectangle, tile };

    /**
     * This object represents a custom sub rectangle in a Tileset image.
     */
    class TilesetRectangle {
        public:
        TilesetRectangle() = default;
        virtual ~TilesetRectangle() = default;

        private:
        int64_t h;
        int64_t tilesetUid;
        int64_t w;
        int64_t x;
        int64_t y;

        public:
        /**
         * Height in pixels
         */
        const int64_t & getH() const { return h; }
        int64_t & getMutableH() { return h; }
        void setH(const int64_t & value) { this->h = value; }

        /**
         * UID of the tileset
         */
        const int64_t & getTilesetUid() const { return tilesetUid; }
        int64_t & getMutableTilesetUid() { return tilesetUid; }
        void setTilesetUid(const int64_t & value) { this->tilesetUid = value; }

        /**
         * Width in pixels
         */
        const int64_t & getW() const { return w; }
        int64_t & getMutableW() { return w; }
        void setW(const int64_t & value) { this->w = value; }

        /**
         * X pixels coordinate of the top-left corner in the Tileset image
         */
        const int64_t & getX() const { return x; }
        int64_t & getMutableX() { return x; }
        void setX(const int64_t & value) { this->x = value; }

        /**
         * Y pixels coordinate of the top-left corner in the Tileset image
         */
        const int64_t & getY() const { return y; }
        int64_t & getMutableY() { return y; }
        void setY(const int64_t & value) { this->y = value; }
    };

    /**
     * An enum describing how the the Entity tile is rendered inside the Entity bounds. Possible
     * values: `Cover`, `FitInside`, `Repeat`, `Stretch`, `FullSizeCropped`,
     * `FullSizeUncropped`, `NineSlice`
     */
    enum class TileRenderMode : int { cover, fitInside, fullSizeCropped, fullSizeUncropped, nineSlice, repeat, stretch };

    class EntityDefinition {
        public:
        EntityDefinition() = default;
        virtual ~EntityDefinition() = default;

        private:
        std::string color;
        std::optional<std::string> doc;
        bool exportToToc;
        std::vector<FieldDefinition> fieldDefs;
        double fillOpacity;
        int64_t height;
        bool hollow;
        std::string identifier;
        bool keepAspectRatio;
        LimitBehavior limitBehavior;
        LimitScope limitScope;
        double lineOpacity;
        int64_t maxCount;
        std::optional<int64_t> maxHeight;
        std::optional<int64_t> maxWidth;
        std::optional<int64_t> minHeight;
        std::optional<int64_t> minWidth;
        std::vector<int64_t> nineSliceBorders;
        double pivotX;
        double pivotY;
        RenderMode renderMode;
        bool resizableX;
        bool resizableY;
        bool showName;
        std::vector<std::string> tags;
        std::optional<int64_t> tileId;
        double tileOpacity;
        std::optional<TilesetRectangle> tileRect;
        TileRenderMode tileRenderMode;
        std::optional<int64_t> tilesetId;
        int64_t uid;
        std::optional<TilesetRectangle> uiTileRect;
        int64_t width;

        public:
        /**
         * Base entity color
         */
        const std::string & getColor() const { return color; }
        std::string & getMutableColor() { return color; }
        void setColor(const std::string & value) { this->color = value; }

        /**
         * User defined documentation for this element to provide help/tips to level designers.
         */
        std::optional<std::string> getDoc() const { return doc; }
        void setDoc(std::optional<std::string> value) { this->doc = value; }

        /**
         * If enabled, all instances of this entity will be listed in the project "Table of content"
         * object.
         */
        const bool & getExportToToc() const { return exportToToc; }
        bool & getMutableExportToToc() { return exportToToc; }
        void setExportToToc(const bool & value) { this->exportToToc = value; }

        /**
         * Array of field definitions
         */
        const std::vector<FieldDefinition> & getFieldDefs() const { return fieldDefs; }
        std::vector<FieldDefinition> & getMutableFieldDefs() { return fieldDefs; }
        void setFieldDefs(const std::vector<FieldDefinition> & value) { this->fieldDefs = value; }

        const double & getFillOpacity() const { return fillOpacity; }
        double & getMutableFillOpacity() { return fillOpacity; }
        void setFillOpacity(const double & value) { this->fillOpacity = value; }

        /**
         * Pixel height
         */
        const int64_t & getHeight() const { return height; }
        int64_t & getMutableHeight() { return height; }
        void setHeight(const int64_t & value) { this->height = value; }

        const bool & getHollow() const { return hollow; }
        bool & getMutableHollow() { return hollow; }
        void setHollow(const bool & value) { this->hollow = value; }

        /**
         * User defined unique identifier
         */
        const std::string & getIdentifier() const { return identifier; }
        std::string & getMutableIdentifier() { return identifier; }
        void setIdentifier(const std::string & value) { this->identifier = value; }

        /**
         * Only applies to entities resizable on both X/Y. If TRUE, the entity instance width/height
         * will keep the same aspect ratio as the definition.
         */
        const bool & getKeepAspectRatio() const { return keepAspectRatio; }
        bool & getMutableKeepAspectRatio() { return keepAspectRatio; }
        void setKeepAspectRatio(const bool & value) { this->keepAspectRatio = value; }

        /**
         * Possible values: `DiscardOldOnes`, `PreventAdding`, `MoveLastOne`
         */
        const LimitBehavior & getLimitBehavior() const { return limitBehavior; }
        LimitBehavior & getMutableLimitBehavior() { return limitBehavior; }
        void setLimitBehavior(const LimitBehavior & value) { this->limitBehavior = value; }

        /**
         * If TRUE, the maxCount is a "per world" limit, if FALSE, it's a "per level". Possible
         * values: `PerLayer`, `PerLevel`, `PerWorld`
         */
        const LimitScope & getLimitScope() const { return limitScope; }
        LimitScope & getMutableLimitScope() { return limitScope; }
        void setLimitScope(const LimitScope & value) { this->limitScope = value; }

        const double & getLineOpacity() const { return lineOpacity; }
        double & getMutableLineOpacity() { return lineOpacity; }
        void setLineOpacity(const double & value) { this->lineOpacity = value; }

        /**
         * Max instances count
         */
        const int64_t & getMaxCount() const { return maxCount; }
        int64_t & getMutableMaxCount() { return maxCount; }
        void setMaxCount(const int64_t & value) { this->maxCount = value; }

        /**
         * Max pixel height (only applies if the entity is resizable on Y)
         */
        std::optional<int64_t> getMaxHeight() const { return maxHeight; }
        void setMaxHeight(std::optional<int64_t> value) { this->maxHeight = value; }

        /**
         * Max pixel width (only applies if the entity is resizable on X)
         */
        std::optional<int64_t> getMaxWidth() const { return maxWidth; }
        void setMaxWidth(std::optional<int64_t> value) { this->maxWidth = value; }

        /**
         * Min pixel height (only applies if the entity is resizable on Y)
         */
        std::optional<int64_t> getMinHeight() const { return minHeight; }
        void setMinHeight(std::optional<int64_t> value) { this->minHeight = value; }

        /**
         * Min pixel width (only applies if the entity is resizable on X)
         */
        std::optional<int64_t> getMinWidth() const { return minWidth; }
        void setMinWidth(std::optional<int64_t> value) { this->minWidth = value; }

        /**
         * An array of 4 dimensions for the up/right/down/left borders (in this order) when using
         * 9-slice mode for `tileRenderMode`.<br/>  If the tileRenderMode is not NineSlice, then
         * this array is empty.<br/>  See: https://en.wikipedia.org/wiki/9-slice_scaling
         */
        const std::vector<int64_t> & getNineSliceBorders() const { return nineSliceBorders; }
        std::vector<int64_t> & getMutableNineSliceBorders() { return nineSliceBorders; }
        void setNineSliceBorders(const std::vector<int64_t> & value) { this->nineSliceBorders = value; }

        /**
         * Pivot X coordinate (from 0 to 1.0)
         */
        const double & getPivotX() const { return pivotX; }
        double & getMutablePivotX() { return pivotX; }
        void setPivotX(const double & value) { this->pivotX = value; }

        /**
         * Pivot Y coordinate (from 0 to 1.0)
         */
        const double & getPivotY() const { return pivotY; }
        double & getMutablePivotY() { return pivotY; }
        void setPivotY(const double & value) { this->pivotY = value; }

        /**
         * Possible values: `Rectangle`, `Ellipse`, `Tile`, `Cross`
         */
        const RenderMode & getRenderMode() const { return renderMode; }
        RenderMode & getMutableRenderMode() { return renderMode; }
        void setRenderMode(const RenderMode & value) { this->renderMode = value; }

        /**
         * If TRUE, the entity instances will be resizable horizontally
         */
        const bool & getResizableX() const { return resizableX; }
        bool & getMutableResizableX() { return resizableX; }
        void setResizableX(const bool & value) { this->resizableX = value; }

        /**
         * If TRUE, the entity instances will be resizable vertically
         */
        const bool & getResizableY() const { return resizableY; }
        bool & getMutableResizableY() { return resizableY; }
        void setResizableY(const bool & value) { this->resizableY = value; }

        /**
         * Display entity name in editor
         */
        const bool & getShowName() const { return showName; }
        bool & getMutableShowName() { return showName; }
        void setShowName(const bool & value) { this->showName = value; }

        /**
         * An array of strings that classifies this entity
         */
        const std::vector<std::string> & getTags() const { return tags; }
        std::vector<std::string> & getMutableTags() { return tags; }
        void setTags(const std::vector<std::string> & value) { this->tags = value; }

        /**
         * **WARNING**: this deprecated value is no longer exported since version 1.2.0  Replaced
         * by: `tileRect`
         */
        std::optional<int64_t> getTileId() const { return tileId; }
        void setTileId(std::optional<int64_t> value) { this->tileId = value; }

        const double & getTileOpacity() const { return tileOpacity; }
        double & getMutableTileOpacity() { return tileOpacity; }
        void setTileOpacity(const double & value) { this->tileOpacity = value; }

        /**
         * An object representing a rectangle from an existing Tileset
         */
        std::optional<TilesetRectangle> getTileRect() const { return tileRect; }
        void setTileRect(std::optional<TilesetRectangle> value) { this->tileRect = value; }

        /**
         * An enum describing how the the Entity tile is rendered inside the Entity bounds. Possible
         * values: `Cover`, `FitInside`, `Repeat`, `Stretch`, `FullSizeCropped`,
         * `FullSizeUncropped`, `NineSlice`
         */
        const TileRenderMode & getTileRenderMode() const { return tileRenderMode; }
        TileRenderMode & getMutableTileRenderMode() { return tileRenderMode; }
        void setTileRenderMode(const TileRenderMode & value) { this->tileRenderMode = value; }

        /**
         * Tileset ID used for optional tile display
         */
        std::optional<int64_t> getTilesetId() const { return tilesetId; }
        void setTilesetId(std::optional<int64_t> value) { this->tilesetId = value; }

        /**
         * Unique Int identifier
         */
        const int64_t & getUid() const { return uid; }
        int64_t & getMutableUid() { return uid; }
        void setUid(const int64_t & value) { this->uid = value; }

        /**
         * This tile overrides the one defined in `tileRect` in the UI
         */
        std::optional<TilesetRectangle> getUiTileRect() const { return uiTileRect; }
        void setUiTileRect(std::optional<TilesetRectangle> value) { this->uiTileRect = value; }

        /**
         * Pixel width
         */
        const int64_t & getWidth() const { return width; }
        int64_t & getMutableWidth() { return width; }
        void setWidth(const int64_t & value) { this->width = value; }
    };

    class EnumValueDefinition {
        public:
        EnumValueDefinition() = default;
        virtual ~EnumValueDefinition() = default;

        private:
        std::optional<std::vector<int64_t>> tileSrcRect;
        int64_t color;
        std::string id;
        std::optional<int64_t> tileId;
        std::optional<TilesetRectangle> tileRect;

        public:
        /**
         * **WARNING**: this deprecated value is no longer exported since version 1.4.0  Replaced
         * by: `tileRect`
         */
        std::optional<std::vector<int64_t>> getTileSrcRect() const { return tileSrcRect; }
        void setTileSrcRect(std::optional<std::vector<int64_t>> value) { this->tileSrcRect = value; }

        /**
         * Optional color
         */
        const int64_t & getColor() const { return color; }
        int64_t & getMutableColor() { return color; }
        void setColor(const int64_t & value) { this->color = value; }

        /**
         * Enum value
         */
        const std::string & getId() const { return id; }
        std::string & getMutableId() { return id; }
        void setId(const std::string & value) { this->id = value; }

        /**
         * **WARNING**: this deprecated value is no longer exported since version 1.4.0  Replaced
         * by: `tileRect`
         */
        std::optional<int64_t> getTileId() const { return tileId; }
        void setTileId(std::optional<int64_t> value) { this->tileId = value; }

        /**
         * Optional tileset rectangle to represents this value
         */
        std::optional<TilesetRectangle> getTileRect() const { return tileRect; }
        void setTileRect(std::optional<TilesetRectangle> value) { this->tileRect = value; }
    };

    class EnumDefinition {
        public:
        EnumDefinition() = default;
        virtual ~EnumDefinition() = default;

        private:
        std::optional<std::string> externalFileChecksum;
        std::optional<std::string> externalRelPath;
        std::optional<int64_t> iconTilesetUid;
        std::string identifier;
        std::vector<std::string> tags;
        int64_t uid;
        std::vector<EnumValueDefinition> values;

        public:
        std::optional<std::string> getExternalFileChecksum() const { return externalFileChecksum; }
        void setExternalFileChecksum(std::optional<std::string> value) { this->externalFileChecksum = value; }

        /**
         * Relative path to the external file providing this Enum
         */
        std::optional<std::string> getExternalRelPath() const { return externalRelPath; }
        void setExternalRelPath(std::optional<std::string> value) { this->externalRelPath = value; }

        /**
         * Tileset UID if provided
         */
        std::optional<int64_t> getIconTilesetUid() const { return iconTilesetUid; }
        void setIconTilesetUid(std::optional<int64_t> value) { this->iconTilesetUid = value; }

        /**
         * User defined unique identifier
         */
        const std::string & getIdentifier() const { return identifier; }
        std::string & getMutableIdentifier() { return identifier; }
        void setIdentifier(const std::string & value) { this->identifier = value; }

        /**
         * An array of user-defined tags to organize the Enums
         */
        const std::vector<std::string> & getTags() const { return tags; }
        std::vector<std::string> & getMutableTags() { return tags; }
        void setTags(const std::vector<std::string> & value) { this->tags = value; }

        /**
         * Unique Int identifier
         */
        const int64_t & getUid() const { return uid; }
        int64_t & getMutableUid() { return uid; }
        void setUid(const int64_t & value) { this->uid = value; }

        /**
         * All possible enum values, with their optional Tile infos.
         */
        const std::vector<EnumValueDefinition> & getValues() const { return values; }
        std::vector<EnumValueDefinition> & getMutableValues() { return values; }
        void setValues(const std::vector<EnumValueDefinition> & value) { this->values = value; }
    };

    /**
     * Checker mode Possible values: `None`, `Horizontal`, `Vertical`
     */
    enum class Checker : int { horizontal, none, vertical };

    /**
     * Defines how tileIds array is used Possible values: `Single`, `Stamp`
     */
    enum class TileMode : int { single, stamp };

    /**
     * This complex section isn't meant to be used by game devs at all, as these rules are
     * completely resolved internally by the editor before any saving. You should just ignore
     * this part.
     */
    class AutoLayerRuleDefinition {
        public:
        AutoLayerRuleDefinition() = default;
        virtual ~AutoLayerRuleDefinition() = default;

        private:
        bool active;
        double alpha;
        bool breakOnMatch;
        double chance;
        Checker checker;
        bool flipX;
        bool flipY;
        std::optional<int64_t> outOfBoundsValue;
        std::vector<int64_t> pattern;
        bool perlinActive;
        double perlinOctaves;
        double perlinScale;
        double perlinSeed;
        double pivotX;
        double pivotY;
        int64_t size;
        std::vector<int64_t> tileIds;
        TileMode tileMode;
        int64_t tileRandomXMax;
        int64_t tileRandomXMin;
        int64_t tileRandomYMax;
        int64_t tileRandomYMin;
        int64_t tileXOffset;
        int64_t tileYOffset;
        int64_t uid;
        int64_t xModulo;
        int64_t xOffset;
        int64_t yModulo;
        int64_t yOffset;

        public:
        /**
         * If FALSE, the rule effect isn't applied, and no tiles are generated.
         */
        const bool & getActive() const { return active; }
        bool & getMutableActive() { return active; }
        void setActive(const bool & value) { this->active = value; }

        const double & getAlpha() const { return alpha; }
        double & getMutableAlpha() { return alpha; }
        void setAlpha(const double & value) { this->alpha = value; }

        /**
         * When TRUE, the rule will prevent other rules to be applied in the same cell if it matches
         * (TRUE by default).
         */
        const bool & getBreakOnMatch() const { return breakOnMatch; }
        bool & getMutableBreakOnMatch() { return breakOnMatch; }
        void setBreakOnMatch(const bool & value) { this->breakOnMatch = value; }

        /**
         * Chances for this rule to be applied (0 to 1)
         */
        const double & getChance() const { return chance; }
        double & getMutableChance() { return chance; }
        void setChance(const double & value) { this->chance = value; }

        /**
         * Checker mode Possible values: `None`, `Horizontal`, `Vertical`
         */
        const Checker & getChecker() const { return checker; }
        Checker & getMutableChecker() { return checker; }
        void setChecker(const Checker & value) { this->checker = value; }

        /**
         * If TRUE, allow rule to be matched by flipping its pattern horizontally
         */
        const bool & getFlipX() const { return flipX; }
        bool & getMutableFlipX() { return flipX; }
        void setFlipX(const bool & value) { this->flipX = value; }

        /**
         * If TRUE, allow rule to be matched by flipping its pattern vertically
         */
        const bool & getFlipY() const { return flipY; }
        bool & getMutableFlipY() { return flipY; }
        void setFlipY(const bool & value) { this->flipY = value; }

        /**
         * Default IntGrid value when checking cells outside of level bounds
         */
        std::optional<int64_t> getOutOfBoundsValue() const { return outOfBoundsValue; }
        void setOutOfBoundsValue(std::optional<int64_t> value) { this->outOfBoundsValue = value; }

        /**
         * Rule pattern (size x size)
         */
        const std::vector<int64_t> & getPattern() const { return pattern; }
        std::vector<int64_t> & getMutablePattern() { return pattern; }
        void setPattern(const std::vector<int64_t> & value) { this->pattern = value; }

        /**
         * If TRUE, enable Perlin filtering to only apply rule on specific random area
         */
        const bool & getPerlinActive() const { return perlinActive; }
        bool & getMutablePerlinActive() { return perlinActive; }
        void setPerlinActive(const bool & value) { this->perlinActive = value; }

        const double & getPerlinOctaves() const { return perlinOctaves; }
        double & getMutablePerlinOctaves() { return perlinOctaves; }
        void setPerlinOctaves(const double & value) { this->perlinOctaves = value; }

        const double & getPerlinScale() const { return perlinScale; }
        double & getMutablePerlinScale() { return perlinScale; }
        void setPerlinScale(const double & value) { this->perlinScale = value; }

        const double & getPerlinSeed() const { return perlinSeed; }
        double & getMutablePerlinSeed() { return perlinSeed; }
        void setPerlinSeed(const double & value) { this->perlinSeed = value; }

        /**
         * X pivot of a tile stamp (0-1)
         */
        const double & getPivotX() const { return pivotX; }
        double & getMutablePivotX() { return pivotX; }
        void setPivotX(const double & value) { this->pivotX = value; }

        /**
         * Y pivot of a tile stamp (0-1)
         */
        const double & getPivotY() const { return pivotY; }
        double & getMutablePivotY() { return pivotY; }
        void setPivotY(const double & value) { this->pivotY = value; }

        /**
         * Pattern width & height. Should only be 1,3,5 or 7.
         */
        const int64_t & getSize() const { return size; }
        int64_t & getMutableSize() { return size; }
        void setSize(const int64_t & value) { this->size = value; }

        /**
         * Array of all the tile IDs. They are used randomly or as stamps, based on `tileMode` value.
         */
        const std::vector<int64_t> & getTileIds() const { return tileIds; }
        std::vector<int64_t> & getMutableTileIds() { return tileIds; }
        void setTileIds(const std::vector<int64_t> & value) { this->tileIds = value; }

        /**
         * Defines how tileIds array is used Possible values: `Single`, `Stamp`
         */
        const TileMode & getTileMode() const { return tileMode; }
        TileMode & getMutableTileMode() { return tileMode; }
        void setTileMode(const TileMode & value) { this->tileMode = value; }

        /**
         * Max random offset for X tile pos
         */
        const int64_t & getTileRandomXMax() const { return tileRandomXMax; }
        int64_t & getMutableTileRandomXMax() { return tileRandomXMax; }
        void setTileRandomXMax(const int64_t & value) { this->tileRandomXMax = value; }

        /**
         * Min random offset for X tile pos
         */
        const int64_t & getTileRandomXMin() const { return tileRandomXMin; }
        int64_t & getMutableTileRandomXMin() { return tileRandomXMin; }
        void setTileRandomXMin(const int64_t & value) { this->tileRandomXMin = value; }

        /**
         * Max random offset for Y tile pos
         */
        const int64_t & getTileRandomYMax() const { return tileRandomYMax; }
        int64_t & getMutableTileRandomYMax() { return tileRandomYMax; }
        void setTileRandomYMax(const int64_t & value) { this->tileRandomYMax = value; }

        /**
         * Min random offset for Y tile pos
         */
        const int64_t & getTileRandomYMin() const { return tileRandomYMin; }
        int64_t & getMutableTileRandomYMin() { return tileRandomYMin; }
        void setTileRandomYMin(const int64_t & value) { this->tileRandomYMin = value; }

        /**
         * Tile X offset
         */
        const int64_t & getTileXOffset() const { return tileXOffset; }
        int64_t & getMutableTileXOffset() { return tileXOffset; }
        void setTileXOffset(const int64_t & value) { this->tileXOffset = value; }

        /**
         * Tile Y offset
         */
        const int64_t & getTileYOffset() const { return tileYOffset; }
        int64_t & getMutableTileYOffset() { return tileYOffset; }
        void setTileYOffset(const int64_t & value) { this->tileYOffset = value; }

        /**
         * Unique Int identifier
         */
        const int64_t & getUid() const { return uid; }
        int64_t & getMutableUid() { return uid; }
        void setUid(const int64_t & value) { this->uid = value; }

        /**
         * X cell coord modulo
         */
        const int64_t & getXModulo() const { return xModulo; }
        int64_t & getMutableXModulo() { return xModulo; }
        void setXModulo(const int64_t & value) { this->xModulo = value; }

        /**
         * X cell start offset
         */
        const int64_t & getXOffset() const { return xOffset; }
        int64_t & getMutableXOffset() { return xOffset; }
        void setXOffset(const int64_t & value) { this->xOffset = value; }

        /**
         * Y cell coord modulo
         */
        const int64_t & getYModulo() const { return yModulo; }
        int64_t & getMutableYModulo() { return yModulo; }
        void setYModulo(const int64_t & value) { this->yModulo = value; }

        /**
         * Y cell start offset
         */
        const int64_t & getYOffset() const { return yOffset; }
        int64_t & getMutableYOffset() { return yOffset; }
        void setYOffset(const int64_t & value) { this->yOffset = value; }
    };

    class AutoLayerRuleGroup {
        public:
        AutoLayerRuleGroup() = default;
        virtual ~AutoLayerRuleGroup() = default;

        private:
        bool active;
        std::optional<bool> collapsed;
        std::optional<std::string> color;
        std::optional<TilesetRectangle> icon;
        bool isOptional;
        std::string name;
        std::vector<AutoLayerRuleDefinition> rules;
        int64_t uid;
        bool usesWizard;

        public:
        const bool & getActive() const { return active; }
        bool & getMutableActive() { return active; }
        void setActive(const bool & value) { this->active = value; }

        /**
         * *This field was removed in 1.0.0 and should no longer be used.*
         */
        std::optional<bool> getCollapsed() const { return collapsed; }
        void setCollapsed(std::optional<bool> value) { this->collapsed = value; }

        std::optional<std::string> getColor() const { return color; }
        void setColor(std::optional<std::string> value) { this->color = value; }

        std::optional<TilesetRectangle> getIcon() const { return icon; }
        void setIcon(std::optional<TilesetRectangle> value) { this->icon = value; }

        const bool & getIsOptional() const { return isOptional; }
        bool & getMutableIsOptional() { return isOptional; }
        void setIsOptional(const bool & value) { this->isOptional = value; }

        const std::string & getName() const { return name; }
        std::string & getMutableName() { return name; }
        void setName(const std::string & value) { this->name = value; }

        const std::vector<AutoLayerRuleDefinition> & getRules() const { return rules; }
        std::vector<AutoLayerRuleDefinition> & getMutableRules() { return rules; }
        void setRules(const std::vector<AutoLayerRuleDefinition> & value) { this->rules = value; }

        const int64_t & getUid() const { return uid; }
        int64_t & getMutableUid() { return uid; }
        void setUid(const int64_t & value) { this->uid = value; }

        const bool & getUsesWizard() const { return usesWizard; }
        bool & getMutableUsesWizard() { return usesWizard; }
        void setUsesWizard(const bool & value) { this->usesWizard = value; }
    };

    /**
     * IntGrid value definition
     */
    class IntGridValueDefinition {
        public:
        IntGridValueDefinition() = default;
        virtual ~IntGridValueDefinition() = default;

        private:
        std::string color;
        int64_t groupUid;
        std::optional<std::string> identifier;
        std::optional<TilesetRectangle> tile;
        int64_t value;

        public:
        const std::string & getColor() const { return color; }
        std::string & getMutableColor() { return color; }
        void setColor(const std::string & value) { this->color = value; }

        /**
         * Parent group identifier (0 if none)
         */
        const int64_t & getGroupUid() const { return groupUid; }
        int64_t & getMutableGroupUid() { return groupUid; }
        void setGroupUid(const int64_t & value) { this->groupUid = value; }

        /**
         * User defined unique identifier
         */
        std::optional<std::string> getIdentifier() const { return identifier; }
        void setIdentifier(std::optional<std::string> value) { this->identifier = value; }

        std::optional<TilesetRectangle> getTile() const { return tile; }
        void setTile(std::optional<TilesetRectangle> value) { this->tile = value; }

        /**
         * The IntGrid value itself
         */
        const int64_t & getValue() const { return value; }
        int64_t & getMutableValue() { return value; }
        void setValue(const int64_t & value) { this->value = value; }
    };

    /**
     * IntGrid value group definition
     */
    class IntGridValueGroupDefinition {
        public:
        IntGridValueGroupDefinition() = default;
        virtual ~IntGridValueGroupDefinition() = default;

        private:
        std::optional<std::string> color;
        std::optional<std::string> identifier;
        int64_t uid;

        public:
        /**
         * User defined color
         */
        std::optional<std::string> getColor() const { return color; }
        void setColor(std::optional<std::string> value) { this->color = value; }

        /**
         * User defined string identifier
         */
        std::optional<std::string> getIdentifier() const { return identifier; }
        void setIdentifier(std::optional<std::string> value) { this->identifier = value; }

        /**
         * Group unique ID
         */
        const int64_t & getUid() const { return uid; }
        int64_t & getMutableUid() { return uid; }
        void setUid(const int64_t & value) { this->uid = value; }
    };

    /**
     * Type of the layer as Haxe Enum Possible values: `IntGrid`, `Entities`, `Tiles`,
     * `AutoLayer`
     */
    enum class Type : int { autoLayer, entities, intGrid, tiles };

    class LayerDefinition {
        public:
        LayerDefinition() = default;
        virtual ~LayerDefinition() = default;

        private:
        std::string type;
        std::vector<AutoLayerRuleGroup> autoRuleGroups;
        std::optional<int64_t> autoSourceLayerDefUid;
        std::optional<int64_t> autoTilesetDefUid;
        bool canSelectWhenInactive;
        double displayOpacity;
        std::optional<std::string> doc;
        std::vector<std::string> excludedTags;
        int64_t gridSize;
        int64_t guideGridHei;
        int64_t guideGridWid;
        bool hideFieldsWhenInactive;
        bool hideInList;
        std::string identifier;
        double inactiveOpacity;
        std::vector<IntGridValueDefinition> intGridValues;
        std::vector<IntGridValueGroupDefinition> intGridValuesGroups;
        double parallaxFactorX;
        double parallaxFactorY;
        bool parallaxScaling;
        int64_t pxOffsetX;
        int64_t pxOffsetY;
        bool renderInWorldView;
        std::vector<std::string> requiredTags;
        double tilePivotX;
        double tilePivotY;
        std::optional<int64_t> tilesetDefUid;
        Type layerDefinitionType;
        std::optional<std::string> uiColor;
        int64_t uid;

        public:
        /**
         * Type of the layer (*IntGrid, Entities, Tiles or AutoLayer*)
         */
        const std::string & getType() const { return type; }
        std::string & getMutableType() { return type; }
        void setType(const std::string & value) { this->type = value; }

        /**
         * Contains all the auto-layer rule definitions.
         */
        const std::vector<AutoLayerRuleGroup> & getAutoRuleGroups() const { return autoRuleGroups; }
        std::vector<AutoLayerRuleGroup> & getMutableAutoRuleGroups() { return autoRuleGroups; }
        void setAutoRuleGroups(const std::vector<AutoLayerRuleGroup> & value) { this->autoRuleGroups = value; }

        std::optional<int64_t> getAutoSourceLayerDefUid() const { return autoSourceLayerDefUid; }
        void setAutoSourceLayerDefUid(std::optional<int64_t> value) { this->autoSourceLayerDefUid = value; }

        /**
         * **WARNING**: this deprecated value is no longer exported since version 1.2.0  Replaced
         * by: `tilesetDefUid`
         */
        std::optional<int64_t> getAutoTilesetDefUid() const { return autoTilesetDefUid; }
        void setAutoTilesetDefUid(std::optional<int64_t> value) { this->autoTilesetDefUid = value; }

        /**
         * Allow editor selections when the layer is not currently active.
         */
        const bool & getCanSelectWhenInactive() const { return canSelectWhenInactive; }
        bool & getMutableCanSelectWhenInactive() { return canSelectWhenInactive; }
        void setCanSelectWhenInactive(const bool & value) { this->canSelectWhenInactive = value; }

        /**
         * Opacity of the layer (0 to 1.0)
         */
        const double & getDisplayOpacity() const { return displayOpacity; }
        double & getMutableDisplayOpacity() { return displayOpacity; }
        void setDisplayOpacity(const double & value) { this->displayOpacity = value; }

        /**
         * User defined documentation for this element to provide help/tips to level designers.
         */
        std::optional<std::string> getDoc() const { return doc; }
        void setDoc(std::optional<std::string> value) { this->doc = value; }

        /**
         * An array of tags to forbid some Entities in this layer
         */
        const std::vector<std::string> & getExcludedTags() const { return excludedTags; }
        std::vector<std::string> & getMutableExcludedTags() { return excludedTags; }
        void setExcludedTags(const std::vector<std::string> & value) { this->excludedTags = value; }

        /**
         * Width and height of the grid in pixels
         */
        const int64_t & getGridSize() const { return gridSize; }
        int64_t & getMutableGridSize() { return gridSize; }
        void setGridSize(const int64_t & value) { this->gridSize = value; }

        /**
         * Height of the optional "guide" grid in pixels
         */
        const int64_t & getGuideGridHei() const { return guideGridHei; }
        int64_t & getMutableGuideGridHei() { return guideGridHei; }
        void setGuideGridHei(const int64_t & value) { this->guideGridHei = value; }

        /**
         * Width of the optional "guide" grid in pixels
         */
        const int64_t & getGuideGridWid() const { return guideGridWid; }
        int64_t & getMutableGuideGridWid() { return guideGridWid; }
        void setGuideGridWid(const int64_t & value) { this->guideGridWid = value; }

        const bool & getHideFieldsWhenInactive() const { return hideFieldsWhenInactive; }
        bool & getMutableHideFieldsWhenInactive() { return hideFieldsWhenInactive; }
        void setHideFieldsWhenInactive(const bool & value) { this->hideFieldsWhenInactive = value; }

        /**
         * Hide the layer from the list on the side of the editor view.
         */
        const bool & getHideInList() const { return hideInList; }
        bool & getMutableHideInList() { return hideInList; }
        void setHideInList(const bool & value) { this->hideInList = value; }

        /**
         * User defined unique identifier
         */
        const std::string & getIdentifier() const { return identifier; }
        std::string & getMutableIdentifier() { return identifier; }
        void setIdentifier(const std::string & value) { this->identifier = value; }

        /**
         * Alpha of this layer when it is not the active one.
         */
        const double & getInactiveOpacity() const { return inactiveOpacity; }
        double & getMutableInactiveOpacity() { return inactiveOpacity; }
        void setInactiveOpacity(const double & value) { this->inactiveOpacity = value; }

        /**
         * An array that defines extra optional info for each IntGrid value.<br/>  WARNING: the
         * array order is not related to actual IntGrid values! As user can re-order IntGrid values
         * freely, you may value "2" before value "1" in this array.
         */
        const std::vector<IntGridValueDefinition> & getIntGridValues() const { return intGridValues; }
        std::vector<IntGridValueDefinition> & getMutableIntGridValues() { return intGridValues; }
        void setIntGridValues(const std::vector<IntGridValueDefinition> & value) { this->intGridValues = value; }

        /**
         * Group informations for IntGrid values
         */
        const std::vector<IntGridValueGroupDefinition> & getIntGridValuesGroups() const { return intGridValuesGroups; }
        std::vector<IntGridValueGroupDefinition> & getMutableIntGridValuesGroups() { return intGridValuesGroups; }
        void setIntGridValuesGroups(const std::vector<IntGridValueGroupDefinition> & value) { this->intGridValuesGroups = value; }

        /**
         * Parallax horizontal factor (from -1 to 1, defaults to 0) which affects the scrolling
         * speed of this layer, creating a fake 3D (parallax) effect.
         */
        const double & getParallaxFactorX() const { return parallaxFactorX; }
        double & getMutableParallaxFactorX() { return parallaxFactorX; }
        void setParallaxFactorX(const double & value) { this->parallaxFactorX = value; }

        /**
         * Parallax vertical factor (from -1 to 1, defaults to 0) which affects the scrolling speed
         * of this layer, creating a fake 3D (parallax) effect.
         */
        const double & getParallaxFactorY() const { return parallaxFactorY; }
        double & getMutableParallaxFactorY() { return parallaxFactorY; }
        void setParallaxFactorY(const double & value) { this->parallaxFactorY = value; }

        /**
         * If true (default), a layer with a parallax factor will also be scaled up/down accordingly.
         */
        const bool & getParallaxScaling() const { return parallaxScaling; }
        bool & getMutableParallaxScaling() { return parallaxScaling; }
        void setParallaxScaling(const bool & value) { this->parallaxScaling = value; }

        /**
         * X offset of the layer, in pixels (IMPORTANT: this should be added to the `LayerInstance`
         * optional offset)
         */
        const int64_t & getPxOffsetX() const { return pxOffsetX; }
        int64_t & getMutablePxOffsetX() { return pxOffsetX; }
        void setPxOffsetX(const int64_t & value) { this->pxOffsetX = value; }

        /**
         * Y offset of the layer, in pixels (IMPORTANT: this should be added to the `LayerInstance`
         * optional offset)
         */
        const int64_t & getPxOffsetY() const { return pxOffsetY; }
        int64_t & getMutablePxOffsetY() { return pxOffsetY; }
        void setPxOffsetY(const int64_t & value) { this->pxOffsetY = value; }

        /**
         * If TRUE, the content of this layer will be used when rendering levels in a simplified way
         * for the world view
         */
        const bool & getRenderInWorldView() const { return renderInWorldView; }
        bool & getMutableRenderInWorldView() { return renderInWorldView; }
        void setRenderInWorldView(const bool & value) { this->renderInWorldView = value; }

        /**
         * An array of tags to filter Entities that can be added to this layer
         */
        const std::vector<std::string> & getRequiredTags() const { return requiredTags; }
        std::vector<std::string> & getMutableRequiredTags() { return requiredTags; }
        void setRequiredTags(const std::vector<std::string> & value) { this->requiredTags = value; }

        /**
         * If the tiles are smaller or larger than the layer grid, the pivot value will be used to
         * position the tile relatively its grid cell.
         */
        const double & getTilePivotX() const { return tilePivotX; }
        double & getMutableTilePivotX() { return tilePivotX; }
        void setTilePivotX(const double & value) { this->tilePivotX = value; }

        /**
         * If the tiles are smaller or larger than the layer grid, the pivot value will be used to
         * position the tile relatively its grid cell.
         */
        const double & getTilePivotY() const { return tilePivotY; }
        double & getMutableTilePivotY() { return tilePivotY; }
        void setTilePivotY(const double & value) { this->tilePivotY = value; }

        /**
         * Reference to the default Tileset UID being used by this layer definition.<br/>
         * **WARNING**: some layer *instances* might use a different tileset. So most of the time,
         * you should probably use the `__tilesetDefUid` value found in layer instances.<br/>  Note:
         * since version 1.0.0, the old `autoTilesetDefUid` was removed and merged into this value.
         */
        std::optional<int64_t> getTilesetDefUid() const { return tilesetDefUid; }
        void setTilesetDefUid(std::optional<int64_t> value) { this->tilesetDefUid = value; }

        /**
         * Type of the layer as Haxe Enum Possible values: `IntGrid`, `Entities`, `Tiles`,
         * `AutoLayer`
         */
        const Type & getLayerDefinitionType() const { return layerDefinitionType; }
        Type & getMutableLayerDefinitionType() { return layerDefinitionType; }
        void setLayerDefinitionType(const Type & value) { this->layerDefinitionType = value; }

        /**
         * User defined color for the UI
         */
        std::optional<std::string> getUiColor() const { return uiColor; }
        void setUiColor(std::optional<std::string> value) { this->uiColor = value; }

        /**
         * Unique Int identifier
         */
        const int64_t & getUid() const { return uid; }
        int64_t & getMutableUid() { return uid; }
        void setUid(const int64_t & value) { this->uid = value; }
    };

    /**
     * In a tileset definition, user defined meta-data of a tile.
     */
    class TileCustomMetadata {
        public:
        TileCustomMetadata() = default;
        virtual ~TileCustomMetadata() = default;

        private:
        std::string data;
        int64_t tileId;

        public:
        const std::string & getData() const { return data; }
        std::string & getMutableData() { return data; }
        void setData(const std::string & value) { this->data = value; }

        const int64_t & getTileId() const { return tileId; }
        int64_t & getMutableTileId() { return tileId; }
        void setTileId(const int64_t & value) { this->tileId = value; }
    };

    enum class EmbedAtlas : int { ldtkIcons };

    /**
     * In a tileset definition, enum based tag infos
     */
    class EnumTagValue {
        public:
        EnumTagValue() = default;
        virtual ~EnumTagValue() = default;

        private:
        std::string enumValueId;
        std::vector<int64_t> tileIds;

        public:
        const std::string & getEnumValueId() const { return enumValueId; }
        std::string & getMutableEnumValueId() { return enumValueId; }
        void setEnumValueId(const std::string & value) { this->enumValueId = value; }

        const std::vector<int64_t> & getTileIds() const { return tileIds; }
        std::vector<int64_t> & getMutableTileIds() { return tileIds; }
        void setTileIds(const std::vector<int64_t> & value) { this->tileIds = value; }
    };

    /**
     * The `Tileset` definition is the most important part among project definitions. It
     * contains some extra informations about each integrated tileset. If you only had to parse
     * one definition section, that would be the one.
     */
    class TilesetDefinition {
        public:
        TilesetDefinition() = default;
        virtual ~TilesetDefinition() = default;

        private:
        int64_t cHei;
        int64_t cWid;
        std::optional<std::map<std::string, nlohmann::json>> cachedPixelData;
        std::vector<TileCustomMetadata> customData;
        std::optional<EmbedAtlas> embedAtlas;
        std::vector<EnumTagValue> enumTags;
        std::string identifier;
        int64_t padding;
        int64_t pxHei;
        int64_t pxWid;
        std::optional<std::string> relPath;
        std::vector<std::map<std::string, nlohmann::json>> savedSelections;
        int64_t spacing;
        std::vector<std::string> tags;
        std::optional<int64_t> tagsSourceEnumUid;
        int64_t tileGridSize;
        int64_t uid;

        public:
        /**
         * Grid-based height
         */
        const int64_t & getCHei() const { return cHei; }
        int64_t & getMutableCHei() { return cHei; }
        void setCHei(const int64_t & value) { this->cHei = value; }

        /**
         * Grid-based width
         */
        const int64_t & getCWid() const { return cWid; }
        int64_t & getMutableCWid() { return cWid; }
        void setCWid(const int64_t & value) { this->cWid = value; }

        /**
         * The following data is used internally for various optimizations. It's always synced with
         * source image changes.
         */
        std::optional<std::map<std::string, nlohmann::json>> getCachedPixelData() const { return cachedPixelData; }
        void setCachedPixelData(std::optional<std::map<std::string, nlohmann::json>> value) { this->cachedPixelData = value; }

        /**
         * An array of custom tile metadata
         */
        const std::vector<TileCustomMetadata> & getCustomData() const { return customData; }
        std::vector<TileCustomMetadata> & getMutableCustomData() { return customData; }
        void setCustomData(const std::vector<TileCustomMetadata> & value) { this->customData = value; }

        /**
         * If this value is set, then it means that this atlas uses an internal LDtk atlas image
         * instead of a loaded one. Possible values: &lt;`null`&gt;, `LdtkIcons`
         */
        std::optional<EmbedAtlas> getEmbedAtlas() const { return embedAtlas; }
        void setEmbedAtlas(std::optional<EmbedAtlas> value) { this->embedAtlas = value; }

        /**
         * Tileset tags using Enum values specified by `tagsSourceEnumId`. This array contains 1
         * element per Enum value, which contains an array of all Tile IDs that are tagged with it.
         */
        const std::vector<EnumTagValue> & getEnumTags() const { return enumTags; }
        std::vector<EnumTagValue> & getMutableEnumTags() { return enumTags; }
        void setEnumTags(const std::vector<EnumTagValue> & value) { this->enumTags = value; }

        /**
         * User defined unique identifier
         */
        const std::string & getIdentifier() const { return identifier; }
        std::string & getMutableIdentifier() { return identifier; }
        void setIdentifier(const std::string & value) { this->identifier = value; }

        /**
         * Distance in pixels from image borders
         */
        const int64_t & getPadding() const { return padding; }
        int64_t & getMutablePadding() { return padding; }
        void setPadding(const int64_t & value) { this->padding = value; }

        /**
         * Image height in pixels
         */
        const int64_t & getPxHei() const { return pxHei; }
        int64_t & getMutablePxHei() { return pxHei; }
        void setPxHei(const int64_t & value) { this->pxHei = value; }

        /**
         * Image width in pixels
         */
        const int64_t & getPxWid() const { return pxWid; }
        int64_t & getMutablePxWid() { return pxWid; }
        void setPxWid(const int64_t & value) { this->pxWid = value; }

        /**
         * Path to the source file, relative to the current project JSON file<br/>  It can be null
         * if no image was provided, or when using an embed atlas.
         */
        std::optional<std::string> getRelPath() const { return relPath; }
        void setRelPath(std::optional<std::string> value) { this->relPath = value; }

        /**
         * Array of group of tiles selections, only meant to be used in the editor
         */
        const std::vector<std::map<std::string, nlohmann::json>> & getSavedSelections() const { return savedSelections; }
        std::vector<std::map<std::string, nlohmann::json>> & getMutableSavedSelections() { return savedSelections; }
        void setSavedSelections(const std::vector<std::map<std::string, nlohmann::json>> & value) { this->savedSelections = value; }

        /**
         * Space in pixels between all tiles
         */
        const int64_t & getSpacing() const { return spacing; }
        int64_t & getMutableSpacing() { return spacing; }
        void setSpacing(const int64_t & value) { this->spacing = value; }

        /**
         * An array of user-defined tags to organize the Tilesets
         */
        const std::vector<std::string> & getTags() const { return tags; }
        std::vector<std::string> & getMutableTags() { return tags; }
        void setTags(const std::vector<std::string> & value) { this->tags = value; }

        /**
         * Optional Enum definition UID used for this tileset meta-data
         */
        std::optional<int64_t> getTagsSourceEnumUid() const { return tagsSourceEnumUid; }
        void setTagsSourceEnumUid(std::optional<int64_t> value) { this->tagsSourceEnumUid = value; }

        const int64_t & getTileGridSize() const { return tileGridSize; }
        int64_t & getMutableTileGridSize() { return tileGridSize; }
        void setTileGridSize(const int64_t & value) { this->tileGridSize = value; }

        /**
         * Unique Intidentifier
         */
        const int64_t & getUid() const { return uid; }
        int64_t & getMutableUid() { return uid; }
        void setUid(const int64_t & value) { this->uid = value; }
    };

    /**
     * If you're writing your own LDtk importer, you should probably just ignore *most* stuff in
     * the `defs` section, as it contains data that are mostly important to the editor. To keep
     * you away from the `defs` section and avoid some unnecessary JSON parsing, important data
     * from definitions is often duplicated in fields prefixed with a double underscore (eg.
     * `__identifier` or `__type`).  The 2 only definition types you might need here are
     * **Tilesets** and **Enums**.
     *
     * A structure containing all the definitions of this project
     */
    class Definitions {
        public:
        Definitions() = default;
        virtual ~Definitions() = default;

        private:
        std::vector<EntityDefinition> entities;
        std::vector<EnumDefinition> enums;
        std::vector<EnumDefinition> externalEnums;
        std::vector<LayerDefinition> layers;
        std::vector<FieldDefinition> levelFields;
        std::vector<TilesetDefinition> tilesets;

        public:
        /**
         * All entities definitions, including their custom fields
         */
        const std::vector<EntityDefinition> & getEntities() const { return entities; }
        std::vector<EntityDefinition> & getMutableEntities() { return entities; }
        void setEntities(const std::vector<EntityDefinition> & value) { this->entities = value; }

        /**
         * All internal enums
         */
        const std::vector<EnumDefinition> & getEnums() const { return enums; }
        std::vector<EnumDefinition> & getMutableEnums() { return enums; }
        void setEnums(const std::vector<EnumDefinition> & value) { this->enums = value; }

        /**
         * Note: external enums are exactly the same as `enums`, except they have a `relPath` to
         * point to an external source file.
         */
        const std::vector<EnumDefinition> & getExternalEnums() const { return externalEnums; }
        std::vector<EnumDefinition> & getMutableExternalEnums() { return externalEnums; }
        void setExternalEnums(const std::vector<EnumDefinition> & value) { this->externalEnums = value; }

        /**
         * All layer definitions
         */
        const std::vector<LayerDefinition> & getLayers() const { return layers; }
        std::vector<LayerDefinition> & getMutableLayers() { return layers; }
        void setLayers(const std::vector<LayerDefinition> & value) { this->layers = value; }

        /**
         * All custom fields available to all levels.
         */
        const std::vector<FieldDefinition> & getLevelFields() const { return levelFields; }
        std::vector<FieldDefinition> & getMutableLevelFields() { return levelFields; }
        void setLevelFields(const std::vector<FieldDefinition> & value) { this->levelFields = value; }

        /**
         * All tilesets
         */
        const std::vector<TilesetDefinition> & getTilesets() const { return tilesets; }
        std::vector<TilesetDefinition> & getMutableTilesets() { return tilesets; }
        void setTilesets(const std::vector<TilesetDefinition> & value) { this->tilesets = value; }
    };

    enum class Flag : int { discardPreCsvIntGrid, exportPreCsvIntGridFormat, ignoreBackupSuggest, multiWorlds, prependIndexToLevelFileNames, useMultilinesType };

    class FieldInstance {
        public:
        FieldInstance() = default;
        virtual ~FieldInstance() = default;

        private:
        std::string identifier;
        std::optional<TilesetRectangle> tile;
        std::string type;
        nlohmann::json value;
        int64_t defUid;
        std::vector<nlohmann::json> realEditorValues;

        public:
        /**
         * Field definition identifier
         */
        const std::string & getIdentifier() const { return identifier; }
        std::string & getMutableIdentifier() { return identifier; }
        void setIdentifier(const std::string & value) { this->identifier = value; }

        /**
         * Optional TilesetRect used to display this field (this can be the field own Tile, or some
         * other Tile guessed from the value, like an Enum).
         */
        std::optional<TilesetRectangle> getTile() const { return tile; }
        void setTile(std::optional<TilesetRectangle> value) { this->tile = value; }

        /**
         * Type of the field, such as `Int`, `Float`, `String`, `Enum(my_enum_name)`, `Bool`,
         * etc.<br/>  NOTE: if you enable the advanced option **Use Multilines type**, you will have
         * "*Multilines*" instead of "*String*" when relevant.
         */
        const std::string & getType() const { return type; }
        std::string & getMutableType() { return type; }
        void setType(const std::string & value) { this->type = value; }

        /**
         * Actual value of the field instance. The value type varies, depending on `__type`:<br/>
         * - For **classic types** (ie. Integer, Float, Boolean, String, Text and FilePath), you
         * just get the actual value with the expected type.<br/>   - For **Color**, the value is an
         * hexadecimal string using "#rrggbb" format.<br/>   - For **Enum**, the value is a String
         * representing the selected enum value.<br/>   - For **Point**, the value is a
         * [GridPoint](#ldtk-GridPoint) object.<br/>   - For **Tile**, the value is a
         * [TilesetRect](#ldtk-TilesetRect) object.<br/>   - For **EntityRef**, the value is an
         * [EntityReferenceInfos](#ldtk-EntityReferenceInfos) object.<br/><br/>  If the field is an
         * array, then this `__value` will also be a JSON array.
         */
        const nlohmann::json & getValue() const { return value; }
        nlohmann::json & getMutableValue() { return value; }
        void setValue(const nlohmann::json & value) { this->value = value; }

        /**
         * Reference of the **Field definition** UID
         */
        const int64_t & getDefUid() const { return defUid; }
        int64_t & getMutableDefUid() { return defUid; }
        void setDefUid(const int64_t & value) { this->defUid = value; }

        /**
         * Editor internal raw values
         */
        const std::vector<nlohmann::json> & getRealEditorValues() const { return realEditorValues; }
        std::vector<nlohmann::json> & getMutableRealEditorValues() { return realEditorValues; }
        void setRealEditorValues(const std::vector<nlohmann::json> & value) { this->realEditorValues = value; }
    };

    class EntityInstance {
        public:
        EntityInstance() = default;
        virtual ~EntityInstance() = default;

        private:
        std::vector<int64_t> grid;
        std::string identifier;
        std::vector<double> pivot;
        std::string smartColor;
        std::vector<std::string> tags;
        std::optional<TilesetRectangle> tile;
        int64_t worldX;
        int64_t worldY;
        int64_t defUid;
        std::vector<FieldInstance> fieldInstances;
        int64_t height;
        std::string iid;
        std::vector<int64_t> px;
        int64_t width;

        public:
        /**
         * Grid-based coordinates (`[x,y]` format)
         */
        const std::vector<int64_t> & getGrid() const { return grid; }
        std::vector<int64_t> & getMutableGrid() { return grid; }
        void setGrid(const std::vector<int64_t> & value) { this->grid = value; }

        /**
         * Entity definition identifier
         */
        const std::string & getIdentifier() const { return identifier; }
        std::string & getMutableIdentifier() { return identifier; }
        void setIdentifier(const std::string & value) { this->identifier = value; }

        /**
         * Pivot coordinates  (`[x,y]` format, values are from 0 to 1) of the Entity
         */
        const std::vector<double> & getPivot() const { return pivot; }
        std::vector<double> & getMutablePivot() { return pivot; }
        void setPivot(const std::vector<double> & value) { this->pivot = value; }

        /**
         * The entity "smart" color, guessed from either Entity definition, or one its field
         * instances.
         */
        const std::string & getSmartColor() const { return smartColor; }
        std::string & getMutableSmartColor() { return smartColor; }
        void setSmartColor(const std::string & value) { this->smartColor = value; }

        /**
         * Array of tags defined in this Entity definition
         */
        const std::vector<std::string> & getTags() const { return tags; }
        std::vector<std::string> & getMutableTags() { return tags; }
        void setTags(const std::vector<std::string> & value) { this->tags = value; }

        /**
         * Optional TilesetRect used to display this entity (it could either be the default Entity
         * tile, or some tile provided by a field value, like an Enum).
         */
        std::optional<TilesetRectangle> getTile() const { return tile; }
        void setTile(std::optional<TilesetRectangle> value) { this->tile = value; }

        /**
         * X world coordinate in pixels
         */
        const int64_t & getWorldX() const { return worldX; }
        int64_t & getMutableWorldX() { return worldX; }
        void setWorldX(const int64_t & value) { this->worldX = value; }

        /**
         * Y world coordinate in pixels
         */
        const int64_t & getWorldY() const { return worldY; }
        int64_t & getMutableWorldY() { return worldY; }
        void setWorldY(const int64_t & value) { this->worldY = value; }

        /**
         * Reference of the **Entity definition** UID
         */
        const int64_t & getDefUid() const { return defUid; }
        int64_t & getMutableDefUid() { return defUid; }
        void setDefUid(const int64_t & value) { this->defUid = value; }

        /**
         * An array of all custom fields and their values.
         */
        const std::vector<FieldInstance> & getFieldInstances() const { return fieldInstances; }
        std::vector<FieldInstance> & getMutableFieldInstances() { return fieldInstances; }
        void setFieldInstances(const std::vector<FieldInstance> & value) { this->fieldInstances = value; }

        /**
         * Entity height in pixels. For non-resizable entities, it will be the same as Entity
         * definition.
         */
        const int64_t & getHeight() const { return height; }
        int64_t & getMutableHeight() { return height; }
        void setHeight(const int64_t & value) { this->height = value; }

        /**
         * Unique instance identifier
         */
        const std::string & getIid() const { return iid; }
        std::string & getMutableIid() { return iid; }
        void setIid(const std::string & value) { this->iid = value; }

        /**
         * Pixel coordinates (`[x,y]` format) in current level coordinate space. Don't forget
         * optional layer offsets, if they exist!
         */
        const std::vector<int64_t> & getPx() const { return px; }
        std::vector<int64_t> & getMutablePx() { return px; }
        void setPx(const std::vector<int64_t> & value) { this->px = value; }

        /**
         * Entity width in pixels. For non-resizable entities, it will be the same as Entity
         * definition.
         */
        const int64_t & getWidth() const { return width; }
        int64_t & getMutableWidth() { return width; }
        void setWidth(const int64_t & value) { this->width = value; }
    };

    /**
     * This object describes the "location" of an Entity instance in the project worlds.
     */
    class ReferenceToAnEntityInstance {
        public:
        ReferenceToAnEntityInstance() = default;
        virtual ~ReferenceToAnEntityInstance() = default;

        private:
        std::string entityIid;
        std::string layerIid;
        std::string levelIid;
        std::string worldIid;

        public:
        /**
         * IID of the refered EntityInstance
         */
        const std::string & getEntityIid() const { return entityIid; }
        std::string & getMutableEntityIid() { return entityIid; }
        void setEntityIid(const std::string & value) { this->entityIid = value; }

        /**
         * IID of the LayerInstance containing the refered EntityInstance
         */
        const std::string & getLayerIid() const { return layerIid; }
        std::string & getMutableLayerIid() { return layerIid; }
        void setLayerIid(const std::string & value) { this->layerIid = value; }

        /**
         * IID of the Level containing the refered EntityInstance
         */
        const std::string & getLevelIid() const { return levelIid; }
        std::string & getMutableLevelIid() { return levelIid; }
        void setLevelIid(const std::string & value) { this->levelIid = value; }

        /**
         * IID of the World containing the refered EntityInstance
         */
        const std::string & getWorldIid() const { return worldIid; }
        std::string & getMutableWorldIid() { return worldIid; }
        void setWorldIid(const std::string & value) { this->worldIid = value; }
    };

    /**
     * This object is just a grid-based coordinate used in Field values.
     */
    class GridPoint {
        public:
        GridPoint() = default;
        virtual ~GridPoint() = default;

        private:
        int64_t cx;
        int64_t cy;

        public:
        /**
         * X grid-based coordinate
         */
        const int64_t & getCx() const { return cx; }
        int64_t & getMutableCx() { return cx; }
        void setCx(const int64_t & value) { this->cx = value; }

        /**
         * Y grid-based coordinate
         */
        const int64_t & getCy() const { return cy; }
        int64_t & getMutableCy() { return cy; }
        void setCy(const int64_t & value) { this->cy = value; }
    };

    /**
     * IntGrid value instance
     */
    class IntGridValueInstance {
        public:
        IntGridValueInstance() = default;
        virtual ~IntGridValueInstance() = default;

        private:
        int64_t coordId;
        int64_t v;

        public:
        /**
         * Coordinate ID in the layer grid
         */
        const int64_t & getCoordId() const { return coordId; }
        int64_t & getMutableCoordId() { return coordId; }
        void setCoordId(const int64_t & value) { this->coordId = value; }

        /**
         * IntGrid value
         */
        const int64_t & getV() const { return v; }
        int64_t & getMutableV() { return v; }
        void setV(const int64_t & value) { this->v = value; }
    };

    /**
     * This structure represents a single tile from a given Tileset.
     */
    class TileInstance {
        public:
        TileInstance() = default;
        virtual ~TileInstance() = default;

        private:
        double a;
        std::vector<int64_t> d;
        int64_t f;
        std::vector<int64_t> px;
        std::vector<int64_t> src;
        int64_t t;

        public:
        /**
         * Alpha/opacity of the tile (0-1, defaults to 1)
         */
        const double & getA() const { return a; }
        double & getMutableA() { return a; }
        void setA(const double & value) { this->a = value; }

        /**
         * Internal data used by the editor.<br/>  For auto-layer tiles: `[ruleId, coordId]`.<br/>
         * For tile-layer tiles: `[coordId]`.
         */
        const std::vector<int64_t> & getD() const { return d; }
        std::vector<int64_t> & getMutableD() { return d; }
        void setD(const std::vector<int64_t> & value) { this->d = value; }

        /**
         * "Flip bits", a 2-bits integer to represent the mirror transformations of the tile.<br/>
         * - Bit 0 = X flip<br/>   - Bit 1 = Y flip<br/>   Examples: f=0 (no flip), f=1 (X flip
         * only), f=2 (Y flip only), f=3 (both flips)
         */
        const int64_t & getF() const { return f; }
        int64_t & getMutableF() { return f; }
        void setF(const int64_t & value) { this->f = value; }

        /**
         * Pixel coordinates of the tile in the **layer** (`[x,y]` format). Don't forget optional
         * layer offsets, if they exist!
         */
        const std::vector<int64_t> & getPx() const { return px; }
        std::vector<int64_t> & getMutablePx() { return px; }
        void setPx(const std::vector<int64_t> & value) { this->px = value; }

        /**
         * Pixel coordinates of the tile in the **tileset** (`[x,y]` format)
         */
        const std::vector<int64_t> & getSrc() const { return src; }
        std::vector<int64_t> & getMutableSrc() { return src; }
        void setSrc(const std::vector<int64_t> & value) { this->src = value; }

        /**
         * The *Tile ID* in the corresponding tileset.
         */
        const int64_t & getT() const { return t; }
        int64_t & getMutableT() { return t; }
        void setT(const int64_t & value) { this->t = value; }
    };

    class LayerInstance {
        public:
        LayerInstance() = default;
        virtual ~LayerInstance() = default;

        private:
        int64_t cHei;
        int64_t cWid;
        int64_t gridSize;
        std::string identifier;
        double opacity;
        int64_t pxTotalOffsetX;
        int64_t pxTotalOffsetY;
        std::optional<int64_t> tilesetDefUid;
        std::optional<std::string> tilesetRelPath;
        std::string type;
        std::vector<TileInstance> autoLayerTiles;
        std::vector<EntityInstance> entityInstances;
        std::vector<TileInstance> gridTiles;
        std::string iid;
        std::optional<std::vector<IntGridValueInstance>> intGrid;
        std::vector<int64_t> intGridCsv;
        int64_t layerDefUid;
        int64_t levelId;
        std::vector<int64_t> optionalRules;
        std::optional<int64_t> overrideTilesetUid;
        int64_t pxOffsetX;
        int64_t pxOffsetY;
        int64_t seed;
        bool visible;

        public:
        /**
         * Grid-based height
         */
        const int64_t & getCHei() const { return cHei; }
        int64_t & getMutableCHei() { return cHei; }
        void setCHei(const int64_t & value) { this->cHei = value; }

        /**
         * Grid-based width
         */
        const int64_t & getCWid() const { return cWid; }
        int64_t & getMutableCWid() { return cWid; }
        void setCWid(const int64_t & value) { this->cWid = value; }

        /**
         * Grid size
         */
        const int64_t & getGridSize() const { return gridSize; }
        int64_t & getMutableGridSize() { return gridSize; }
        void setGridSize(const int64_t & value) { this->gridSize = value; }

        /**
         * Layer definition identifier
         */
        const std::string & getIdentifier() const { return identifier; }
        std::string & getMutableIdentifier() { return identifier; }
        void setIdentifier(const std::string & value) { this->identifier = value; }

        /**
         * Layer opacity as Float [0-1]
         */
        const double & getOpacity() const { return opacity; }
        double & getMutableOpacity() { return opacity; }
        void setOpacity(const double & value) { this->opacity = value; }

        /**
         * Total layer X pixel offset, including both instance and definition offsets.
         */
        const int64_t & getPxTotalOffsetX() const { return pxTotalOffsetX; }
        int64_t & getMutablePxTotalOffsetX() { return pxTotalOffsetX; }
        void setPxTotalOffsetX(const int64_t & value) { this->pxTotalOffsetX = value; }

        /**
         * Total layer Y pixel offset, including both instance and definition offsets.
         */
        const int64_t & getPxTotalOffsetY() const { return pxTotalOffsetY; }
        int64_t & getMutablePxTotalOffsetY() { return pxTotalOffsetY; }
        void setPxTotalOffsetY(const int64_t & value) { this->pxTotalOffsetY = value; }

        /**
         * The definition UID of corresponding Tileset, if any.
         */
        std::optional<int64_t> getTilesetDefUid() const { return tilesetDefUid; }
        void setTilesetDefUid(std::optional<int64_t> value) { this->tilesetDefUid = value; }

        /**
         * The relative path to corresponding Tileset, if any.
         */
        std::optional<std::string> getTilesetRelPath() const { return tilesetRelPath; }
        void setTilesetRelPath(std::optional<std::string> value) { this->tilesetRelPath = value; }

        /**
         * Layer type (possible values: IntGrid, Entities, Tiles or AutoLayer)
         */
        const std::string & getType() const { return type; }
        std::string & getMutableType() { return type; }
        void setType(const std::string & value) { this->type = value; }

        /**
         * An array containing all tiles generated by Auto-layer rules. The array is already sorted
         * in display order (ie. 1st tile is beneath 2nd, which is beneath 3rd etc.).<br/><br/>
         * Note: if multiple tiles are stacked in the same cell as the result of different rules,
         * all tiles behind opaque ones will be discarded.
         */
        const std::vector<TileInstance> & getAutoLayerTiles() const { return autoLayerTiles; }
        std::vector<TileInstance> & getMutableAutoLayerTiles() { return autoLayerTiles; }
        void setAutoLayerTiles(const std::vector<TileInstance> & value) { this->autoLayerTiles = value; }

        const std::vector<EntityInstance> & getEntityInstances() const { return entityInstances; }
        std::vector<EntityInstance> & getMutableEntityInstances() { return entityInstances; }
        void setEntityInstances(const std::vector<EntityInstance> & value) { this->entityInstances = value; }

        const std::vector<TileInstance> & getGridTiles() const { return gridTiles; }
        std::vector<TileInstance> & getMutableGridTiles() { return gridTiles; }
        void setGridTiles(const std::vector<TileInstance> & value) { this->gridTiles = value; }

        /**
         * Unique layer instance identifier
         */
        const std::string & getIid() const { return iid; }
        std::string & getMutableIid() { return iid; }
        void setIid(const std::string & value) { this->iid = value; }

        /**
         * **WARNING**: this deprecated value is no longer exported since version 1.0.0  Replaced
         * by: `intGridCsv`
         */
        std::optional<std::vector<IntGridValueInstance>> getIntGrid() const { return intGrid; }
        void setIntGrid(std::optional<std::vector<IntGridValueInstance>> value) { this->intGrid = value; }

        /**
         * A list of all values in the IntGrid layer, stored in CSV format (Comma Separated
         * Values).<br/>  Order is from left to right, and top to bottom (ie. first row from left to
         * right, followed by second row, etc).<br/>  `0` means "empty cell" and IntGrid values
         * start at 1.<br/>  The array size is `__cWid` x `__cHei` cells.
         */
        const std::vector<int64_t> & getIntGridCsv() const { return intGridCsv; }
        std::vector<int64_t> & getMutableIntGridCsv() { return intGridCsv; }
        void setIntGridCsv(const std::vector<int64_t> & value) { this->intGridCsv = value; }

        /**
         * Reference the Layer definition UID
         */
        const int64_t & getLayerDefUid() const { return layerDefUid; }
        int64_t & getMutableLayerDefUid() { return layerDefUid; }
        void setLayerDefUid(const int64_t & value) { this->layerDefUid = value; }

        /**
         * Reference to the UID of the level containing this layer instance
         */
        const int64_t & getLevelId() const { return levelId; }
        int64_t & getMutableLevelId() { return levelId; }
        void setLevelId(const int64_t & value) { this->levelId = value; }

        /**
         * An Array containing the UIDs of optional rules that were enabled in this specific layer
         * instance.
         */
        const std::vector<int64_t> & getOptionalRules() const { return optionalRules; }
        std::vector<int64_t> & getMutableOptionalRules() { return optionalRules; }
        void setOptionalRules(const std::vector<int64_t> & value) { this->optionalRules = value; }

        /**
         * This layer can use another tileset by overriding the tileset UID here.
         */
        std::optional<int64_t> getOverrideTilesetUid() const { return overrideTilesetUid; }
        void setOverrideTilesetUid(std::optional<int64_t> value) { this->overrideTilesetUid = value; }

        /**
         * X offset in pixels to render this layer, usually 0 (IMPORTANT: this should be added to
         * the `LayerDef` optional offset, so you should probably prefer using `__pxTotalOffsetX`
         * which contains the total offset value)
         */
        const int64_t & getPxOffsetX() const { return pxOffsetX; }
        int64_t & getMutablePxOffsetX() { return pxOffsetX; }
        void setPxOffsetX(const int64_t & value) { this->pxOffsetX = value; }

        /**
         * Y offset in pixels to render this layer, usually 0 (IMPORTANT: this should be added to
         * the `LayerDef` optional offset, so you should probably prefer using `__pxTotalOffsetX`
         * which contains the total offset value)
         */
        const int64_t & getPxOffsetY() const { return pxOffsetY; }
        int64_t & getMutablePxOffsetY() { return pxOffsetY; }
        void setPxOffsetY(const int64_t & value) { this->pxOffsetY = value; }

        /**
         * Random seed used for Auto-Layers rendering
         */
        const int64_t & getSeed() const { return seed; }
        int64_t & getMutableSeed() { return seed; }
        void setSeed(const int64_t & value) { this->seed = value; }

        /**
         * Layer instance visibility
         */
        const bool & getVisible() const { return visible; }
        bool & getMutableVisible() { return visible; }
        void setVisible(const bool & value) { this->visible = value; }
    };

    /**
     * Level background image position info
     */
    class LevelBackgroundPosition {
        public:
        LevelBackgroundPosition() = default;
        virtual ~LevelBackgroundPosition() = default;

        private:
        std::vector<double> cropRect;
        std::vector<double> scale;
        std::vector<int64_t> topLeftPx;

        public:
        /**
         * An array of 4 float values describing the cropped sub-rectangle of the displayed
         * background image. This cropping happens when original is larger than the level bounds.
         * Array format: `[ cropX, cropY, cropWidth, cropHeight ]`
         */
        const std::vector<double> & getCropRect() const { return cropRect; }
        std::vector<double> & getMutableCropRect() { return cropRect; }
        void setCropRect(const std::vector<double> & value) { this->cropRect = value; }

        /**
         * An array containing the `[scaleX,scaleY]` values of the **cropped** background image,
         * depending on `bgPos` option.
         */
        const std::vector<double> & getScale() const { return scale; }
        std::vector<double> & getMutableScale() { return scale; }
        void setScale(const std::vector<double> & value) { this->scale = value; }

        /**
         * An array containing the `[x,y]` pixel coordinates of the top-left corner of the
         * **cropped** background image, depending on `bgPos` option.
         */
        const std::vector<int64_t> & getTopLeftPx() const { return topLeftPx; }
        std::vector<int64_t> & getMutableTopLeftPx() { return topLeftPx; }
        void setTopLeftPx(const std::vector<int64_t> & value) { this->topLeftPx = value; }
    };

    enum class BgPos : int { contain, cover, coverDirty, repeat, unscaled };

    /**
     * Nearby level info
     */
    class NeighbourLevel {
        public:
        NeighbourLevel() = default;
        virtual ~NeighbourLevel() = default;

        private:
        std::string dir;
        std::string levelIid;
        std::optional<int64_t> levelUid;

        public:
        /**
         * A single lowercase character tipping on the level location (`n`orth, `s`outh, `w`est,
         * `e`ast).<br/>  Since 1.4.0, this character value can also be `<` (neighbour depth is
         * lower), `>` (neighbour depth is greater) or `o` (levels overlap and share the same world
         * depth).
         */
        const std::string & getDir() const { return dir; }
        std::string & getMutableDir() { return dir; }
        void setDir(const std::string & value) { this->dir = value; }

        /**
         * Neighbour Instance Identifier
         */
        const std::string & getLevelIid() const { return levelIid; }
        std::string & getMutableLevelIid() { return levelIid; }
        void setLevelIid(const std::string & value) { this->levelIid = value; }

        /**
         * **WARNING**: this deprecated value is no longer exported since version 1.2.0  Replaced
         * by: `levelIid`
         */
        std::optional<int64_t> getLevelUid() const { return levelUid; }
        void setLevelUid(std::optional<int64_t> value) { this->levelUid = value; }
    };

    /**
     * This section contains all the level data. It can be found in 2 distinct forms, depending
     * on Project current settings:  - If "*Separate level files*" is **disabled** (default):
     * full level data is *embedded* inside the main Project JSON file, - If "*Separate level
     * files*" is **enabled**: level data is stored in *separate* standalone `.ldtkl` files (one
     * per level). In this case, the main Project JSON file will still contain most level data,
     * except heavy sections, like the `layerInstances` array (which will be null). The
     * `externalRelPath` string points to the `ldtkl` file.  A `ldtkl` file is just a JSON file
     * containing exactly what is described below.
     */
    class Level {
        public:
        Level() = default;
        virtual ~Level() = default;

        private:
        std::string bgColor;
        std::optional<LevelBackgroundPosition> bgPos;
        std::vector<NeighbourLevel> neighbours;
        std::string smartColor;
        std::optional<std::string> levelBgColor;
        double bgPivotX;
        double bgPivotY;
        std::optional<BgPos> levelBgPos;
        std::optional<std::string> bgRelPath;
        std::optional<std::string> externalRelPath;
        std::vector<FieldInstance> fieldInstances;
        std::string identifier;
        std::string iid;
        std::optional<std::vector<LayerInstance>> layerInstances;
        int64_t pxHei;
        int64_t pxWid;
        int64_t uid;
        bool useAutoIdentifier;
        int64_t worldDepth;
        int64_t worldX;
        int64_t worldY;

        public:
        /**
         * Background color of the level (same as `bgColor`, except the default value is
         * automatically used here if its value is `null`)
         */
        const std::string & getBgColor() const { return bgColor; }
        std::string & getMutableBgColor() { return bgColor; }
        void setBgColor(const std::string & value) { this->bgColor = value; }

        /**
         * Position informations of the background image, if there is one.
         */
        std::optional<LevelBackgroundPosition> getBgPos() const { return bgPos; }
        void setBgPos(std::optional<LevelBackgroundPosition> value) { this->bgPos = value; }

        /**
         * An array listing all other levels touching this one on the world map. Since 1.4.0, this
         * includes levels that overlap in the same world layer, or in nearby world layers.<br/>
         * Only relevant for world layouts where level spatial positioning is manual (ie. GridVania,
         * Free). For Horizontal and Vertical layouts, this array is always empty.
         */
        const std::vector<NeighbourLevel> & getNeighbours() const { return neighbours; }
        std::vector<NeighbourLevel> & getMutableNeighbours() { return neighbours; }
        void setNeighbours(const std::vector<NeighbourLevel> & value) { this->neighbours = value; }

        /**
         * The "guessed" color for this level in the editor, decided using either the background
         * color or an existing custom field.
         */
        const std::string & getSmartColor() const { return smartColor; }
        std::string & getMutableSmartColor() { return smartColor; }
        void setSmartColor(const std::string & value) { this->smartColor = value; }

        /**
         * Background color of the level. If `null`, the project `defaultLevelBgColor` should be
         * used.
         */
        std::optional<std::string> getLevelBgColor() const { return levelBgColor; }
        void setLevelBgColor(std::optional<std::string> value) { this->levelBgColor = value; }

        /**
         * Background image X pivot (0-1)
         */
        const double & getBgPivotX() const { return bgPivotX; }
        double & getMutableBgPivotX() { return bgPivotX; }
        void setBgPivotX(const double & value) { this->bgPivotX = value; }

        /**
         * Background image Y pivot (0-1)
         */
        const double & getBgPivotY() const { return bgPivotY; }
        double & getMutableBgPivotY() { return bgPivotY; }
        void setBgPivotY(const double & value) { this->bgPivotY = value; }

        /**
         * An enum defining the way the background image (if any) is positioned on the level. See
         * `__bgPos` for resulting position info. Possible values: &lt;`null`&gt;, `Unscaled`,
         * `Contain`, `Cover`, `CoverDirty`, `Repeat`
         */
        std::optional<BgPos> getLevelBgPos() const { return levelBgPos; }
        void setLevelBgPos(std::optional<BgPos> value) { this->levelBgPos = value; }

        /**
         * The *optional* relative path to the level background image.
         */
        std::optional<std::string> getBgRelPath() const { return bgRelPath; }
        void setBgRelPath(std::optional<std::string> value) { this->bgRelPath = value; }

        /**
         * This value is not null if the project option "*Save levels separately*" is enabled. In
         * this case, this **relative** path points to the level Json file.
         */
        std::optional<std::string> getExternalRelPath() const { return externalRelPath; }
        void setExternalRelPath(std::optional<std::string> value) { this->externalRelPath = value; }

        /**
         * An array containing this level custom field values.
         */
        const std::vector<FieldInstance> & getFieldInstances() const { return fieldInstances; }
        std::vector<FieldInstance> & getMutableFieldInstances() { return fieldInstances; }
        void setFieldInstances(const std::vector<FieldInstance> & value) { this->fieldInstances = value; }

        /**
         * User defined unique identifier
         */
        const std::string & getIdentifier() const { return identifier; }
        std::string & getMutableIdentifier() { return identifier; }
        void setIdentifier(const std::string & value) { this->identifier = value; }

        /**
         * Unique instance identifier
         */
        const std::string & getIid() const { return iid; }
        std::string & getMutableIid() { return iid; }
        void setIid(const std::string & value) { this->iid = value; }

        /**
         * An array containing all Layer instances. **IMPORTANT**: if the project option "*Save
         * levels separately*" is enabled, this field will be `null`.<br/>  This array is **sorted
         * in display order**: the 1st layer is the top-most and the last is behind.
         */
        std::optional<std::vector<LayerInstance>> getLayerInstances() const { return layerInstances; }
        void setLayerInstances(std::optional<std::vector<LayerInstance>> value) { this->layerInstances = value; }

        /**
         * Height of the level in pixels
         */
        const int64_t & getPxHei() const { return pxHei; }
        int64_t & getMutablePxHei() { return pxHei; }
        void setPxHei(const int64_t & value) { this->pxHei = value; }

        /**
         * Width of the level in pixels
         */
        const int64_t & getPxWid() const { return pxWid; }
        int64_t & getMutablePxWid() { return pxWid; }
        void setPxWid(const int64_t & value) { this->pxWid = value; }

        /**
         * Unique Int identifier
         */
        const int64_t & getUid() const { return uid; }
        int64_t & getMutableUid() { return uid; }
        void setUid(const int64_t & value) { this->uid = value; }

        /**
         * If TRUE, the level identifier will always automatically use the naming pattern as defined
         * in `Project.levelNamePattern`. Becomes FALSE if the identifier is manually modified by
         * user.
         */
        const bool & getUseAutoIdentifier() const { return useAutoIdentifier; }
        bool & getMutableUseAutoIdentifier() { return useAutoIdentifier; }
        void setUseAutoIdentifier(const bool & value) { this->useAutoIdentifier = value; }

        /**
         * Index that represents the "depth" of the level in the world. Default is 0, greater means
         * "above", lower means "below".<br/>  This value is mostly used for display only and is
         * intended to make stacking of levels easier to manage.
         */
        const int64_t & getWorldDepth() const { return worldDepth; }
        int64_t & getMutableWorldDepth() { return worldDepth; }
        void setWorldDepth(const int64_t & value) { this->worldDepth = value; }

        /**
         * World X coordinate in pixels.<br/>  Only relevant for world layouts where level spatial
         * positioning is manual (ie. GridVania, Free). For Horizontal and Vertical layouts, the
         * value is always -1 here.
         */
        const int64_t & getWorldX() const { return worldX; }
        int64_t & getMutableWorldX() { return worldX; }
        void setWorldX(const int64_t & value) { this->worldX = value; }

        /**
         * World Y coordinate in pixels.<br/>  Only relevant for world layouts where level spatial
         * positioning is manual (ie. GridVania, Free). For Horizontal and Vertical layouts, the
         * value is always -1 here.
         */
        const int64_t & getWorldY() const { return worldY; }
        int64_t & getMutableWorldY() { return worldY; }
        void setWorldY(const int64_t & value) { this->worldY = value; }
    };

    class LdtkTableOfContentEntry {
        public:
        LdtkTableOfContentEntry() = default;
        virtual ~LdtkTableOfContentEntry() = default;

        private:
        std::string identifier;
        std::vector<ReferenceToAnEntityInstance> instances;

        public:
        const std::string & getIdentifier() const { return identifier; }
        std::string & getMutableIdentifier() { return identifier; }
        void setIdentifier(const std::string & value) { this->identifier = value; }

        const std::vector<ReferenceToAnEntityInstance> & getInstances() const { return instances; }
        std::vector<ReferenceToAnEntityInstance> & getMutableInstances() { return instances; }
        void setInstances(const std::vector<ReferenceToAnEntityInstance> & value) { this->instances = value; }
    };

    enum class WorldLayout : int { free, gridVania, linearHorizontal, linearVertical };

    /**
     * **IMPORTANT**: this type is available as a preview. You can rely on it to update your
     * importers, for when it will be officially available.  A World contains multiple levels,
     * and it has its own layout settings.
     */
    class World {
        public:
        World() = default;
        virtual ~World() = default;

        private:
        int64_t defaultLevelHeight;
        int64_t defaultLevelWidth;
        std::string identifier;
        std::string iid;
        std::vector<Level> levels;
        int64_t worldGridHeight;
        int64_t worldGridWidth;
        std::optional<WorldLayout> worldLayout;

        public:
        /**
         * Default new level height
         */
        const int64_t & getDefaultLevelHeight() const { return defaultLevelHeight; }
        int64_t & getMutableDefaultLevelHeight() { return defaultLevelHeight; }
        void setDefaultLevelHeight(const int64_t & value) { this->defaultLevelHeight = value; }

        /**
         * Default new level width
         */
        const int64_t & getDefaultLevelWidth() const { return defaultLevelWidth; }
        int64_t & getMutableDefaultLevelWidth() { return defaultLevelWidth; }
        void setDefaultLevelWidth(const int64_t & value) { this->defaultLevelWidth = value; }

        /**
         * User defined unique identifier
         */
        const std::string & getIdentifier() const { return identifier; }
        std::string & getMutableIdentifier() { return identifier; }
        void setIdentifier(const std::string & value) { this->identifier = value; }

        /**
         * Unique instance identifer
         */
        const std::string & getIid() const { return iid; }
        std::string & getMutableIid() { return iid; }
        void setIid(const std::string & value) { this->iid = value; }

        /**
         * All levels from this world. The order of this array is only relevant in
         * `LinearHorizontal` and `linearVertical` world layouts (see `worldLayout` value).
         * Otherwise, you should refer to the `worldX`,`worldY` coordinates of each Level.
         */
        const std::vector<Level> & getLevels() const { return levels; }
        std::vector<Level> & getMutableLevels() { return levels; }
        void setLevels(const std::vector<Level> & value) { this->levels = value; }

        /**
         * Height of the world grid in pixels.
         */
        const int64_t & getWorldGridHeight() const { return worldGridHeight; }
        int64_t & getMutableWorldGridHeight() { return worldGridHeight; }
        void setWorldGridHeight(const int64_t & value) { this->worldGridHeight = value; }

        /**
         * Width of the world grid in pixels.
         */
        const int64_t & getWorldGridWidth() const { return worldGridWidth; }
        int64_t & getMutableWorldGridWidth() { return worldGridWidth; }
        void setWorldGridWidth(const int64_t & value) { this->worldGridWidth = value; }

        /**
         * An enum that describes how levels are organized in this project (ie. linearly or in a 2D
         * space). Possible values: `Free`, `GridVania`, `LinearHorizontal`, `LinearVertical`, `null`
         */
        std::optional<WorldLayout> getWorldLayout() const { return worldLayout; }
        void setWorldLayout(std::optional<WorldLayout> value) { this->worldLayout = value; }
    };

    /**
     * This object is not actually used by LDtk. It ONLY exists to force explicit references to
     * all types, to make sure QuickType finds them and integrate all of them. Otherwise,
     * Quicktype will drop types that are not explicitely used.
     */
    class ForcedRefs {
        public:
        ForcedRefs() = default;
        virtual ~ForcedRefs() = default;

        private:
        std::optional<AutoLayerRuleGroup> autoLayerRuleGroup;
        std::optional<AutoLayerRuleDefinition> autoRuleDef;
        std::optional<LdtkCustomCommand> customCommand;
        std::optional<Definitions> definitions;
        std::optional<EntityDefinition> entityDef;
        std::optional<EntityInstance> entityInstance;
        std::optional<ReferenceToAnEntityInstance> entityReferenceInfos;
        std::optional<EnumDefinition> enumDef;
        std::optional<EnumValueDefinition> enumDefValues;
        std::optional<EnumTagValue> enumTagValue;
        std::optional<FieldDefinition> fieldDef;
        std::optional<FieldInstance> fieldInstance;
        std::optional<GridPoint> gridPoint;
        std::optional<IntGridValueDefinition> intGridValueDef;
        std::optional<IntGridValueGroupDefinition> intGridValueGroupDef;
        std::optional<IntGridValueInstance> intGridValueInstance;
        std::optional<LayerDefinition> layerDef;
        std::optional<LayerInstance> layerInstance;
        std::optional<Level> level;
        std::optional<LevelBackgroundPosition> levelBgPosInfos;
        std::optional<NeighbourLevel> neighbourLevel;
        std::optional<LdtkTableOfContentEntry> tableOfContentEntry;
        std::optional<TileInstance> tile;
        std::optional<TileCustomMetadata> tileCustomMetadata;
        std::optional<TilesetDefinition> tilesetDef;
        std::optional<TilesetRectangle> tilesetRect;
        std::optional<World> world;

        public:
        std::optional<AutoLayerRuleGroup> getAutoLayerRuleGroup() const { return autoLayerRuleGroup; }
        void setAutoLayerRuleGroup(std::optional<AutoLayerRuleGroup> value) { this->autoLayerRuleGroup = value; }

        std::optional<AutoLayerRuleDefinition> getAutoRuleDef() const { return autoRuleDef; }
        void setAutoRuleDef(std::optional<AutoLayerRuleDefinition> value) { this->autoRuleDef = value; }

        std::optional<LdtkCustomCommand> getCustomCommand() const { return customCommand; }
        void setCustomCommand(std::optional<LdtkCustomCommand> value) { this->customCommand = value; }

        std::optional<Definitions> getDefinitions() const { return definitions; }
        void setDefinitions(std::optional<Definitions> value) { this->definitions = value; }

        std::optional<EntityDefinition> getEntityDef() const { return entityDef; }
        void setEntityDef(std::optional<EntityDefinition> value) { this->entityDef = value; }

        std::optional<EntityInstance> getEntityInstance() const { return entityInstance; }
        void setEntityInstance(std::optional<EntityInstance> value) { this->entityInstance = value; }

        std::optional<ReferenceToAnEntityInstance> getEntityReferenceInfos() const { return entityReferenceInfos; }
        void setEntityReferenceInfos(std::optional<ReferenceToAnEntityInstance> value) { this->entityReferenceInfos = value; }

        std::optional<EnumDefinition> getEnumDef() const { return enumDef; }
        void setEnumDef(std::optional<EnumDefinition> value) { this->enumDef = value; }

        std::optional<EnumValueDefinition> getEnumDefValues() const { return enumDefValues; }
        void setEnumDefValues(std::optional<EnumValueDefinition> value) { this->enumDefValues = value; }

        std::optional<EnumTagValue> getEnumTagValue() const { return enumTagValue; }
        void setEnumTagValue(std::optional<EnumTagValue> value) { this->enumTagValue = value; }

        std::optional<FieldDefinition> getFieldDef() const { return fieldDef; }
        void setFieldDef(std::optional<FieldDefinition> value) { this->fieldDef = value; }

        std::optional<FieldInstance> getFieldInstance() const { return fieldInstance; }
        void setFieldInstance(std::optional<FieldInstance> value) { this->fieldInstance = value; }

        std::optional<GridPoint> getGridPoint() const { return gridPoint; }
        void setGridPoint(std::optional<GridPoint> value) { this->gridPoint = value; }

        std::optional<IntGridValueDefinition> getIntGridValueDef() const { return intGridValueDef; }
        void setIntGridValueDef(std::optional<IntGridValueDefinition> value) { this->intGridValueDef = value; }

        std::optional<IntGridValueGroupDefinition> getIntGridValueGroupDef() const { return intGridValueGroupDef; }
        void setIntGridValueGroupDef(std::optional<IntGridValueGroupDefinition> value) { this->intGridValueGroupDef = value; }

        std::optional<IntGridValueInstance> getIntGridValueInstance() const { return intGridValueInstance; }
        void setIntGridValueInstance(std::optional<IntGridValueInstance> value) { this->intGridValueInstance = value; }

        std::optional<LayerDefinition> getLayerDef() const { return layerDef; }
        void setLayerDef(std::optional<LayerDefinition> value) { this->layerDef = value; }

        std::optional<LayerInstance> getLayerInstance() const { return layerInstance; }
        void setLayerInstance(std::optional<LayerInstance> value) { this->layerInstance = value; }

        std::optional<Level> getLevel() const { return level; }
        void setLevel(std::optional<Level> value) { this->level = value; }

        std::optional<LevelBackgroundPosition> getLevelBgPosInfos() const { return levelBgPosInfos; }
        void setLevelBgPosInfos(std::optional<LevelBackgroundPosition> value) { this->levelBgPosInfos = value; }

        std::optional<NeighbourLevel> getNeighbourLevel() const { return neighbourLevel; }
        void setNeighbourLevel(std::optional<NeighbourLevel> value) { this->neighbourLevel = value; }

        std::optional<LdtkTableOfContentEntry> getTableOfContentEntry() const { return tableOfContentEntry; }
        void setTableOfContentEntry(std::optional<LdtkTableOfContentEntry> value) { this->tableOfContentEntry = value; }

        std::optional<TileInstance> getTile() const { return tile; }
        void setTile(std::optional<TileInstance> value) { this->tile = value; }

        std::optional<TileCustomMetadata> getTileCustomMetadata() const { return tileCustomMetadata; }
        void setTileCustomMetadata(std::optional<TileCustomMetadata> value) { this->tileCustomMetadata = value; }

        std::optional<TilesetDefinition> getTilesetDef() const { return tilesetDef; }
        void setTilesetDef(std::optional<TilesetDefinition> value) { this->tilesetDef = value; }

        std::optional<TilesetRectangle> getTilesetRect() const { return tilesetRect; }
        void setTilesetRect(std::optional<TilesetRectangle> value) { this->tilesetRect = value; }

        std::optional<World> getWorld() const { return world; }
        void setWorld(std::optional<World> value) { this->world = value; }
    };

    /**
     * Naming convention for Identifiers (first-letter uppercase, full uppercase etc.) Possible
     * values: `Capitalize`, `Uppercase`, `Lowercase`, `Free`
     */
    enum class IdentifierStyle : int { capitalize, free, lowercase, uppercase };

    /**
     * "Image export" option when saving project. Possible values: `None`, `OneImagePerLayer`,
     * `OneImagePerLevel`, `LayersAndLevels`
     */
    enum class ImageExportMode : int { layersAndLevels, none, oneImagePerLayer, oneImagePerLevel };

    /**
     * This file is a JSON schema of files created by LDtk level editor (https://ldtk.io).
     *
     * This is the root of any Project JSON file. It contains:  - the project settings, - an
     * array of levels, - a group of definitions (that can probably be safely ignored for most
     * users).
     */
    class Coordinate {
        public:
        Coordinate() = default;
        virtual ~Coordinate() = default;

        private:
        std::optional<ForcedRefs> forcedRefs;
        double appBuildId;
        int64_t backupLimit;
        bool backupOnSave;
        std::optional<std::string> backupRelPath;
        std::string bgColor;
        std::vector<LdtkCustomCommand> customCommands;
        int64_t defaultEntityHeight;
        int64_t defaultEntityWidth;
        int64_t defaultGridSize;
        std::string defaultLevelBgColor;
        std::optional<int64_t> defaultLevelHeight;
        std::optional<int64_t> defaultLevelWidth;
        double defaultPivotX;
        double defaultPivotY;
        Definitions defs;
        std::string dummyWorldIid;
        bool exportLevelBg;
        std::optional<bool> exportPng;
        bool exportTiled;
        bool externalLevels;
        std::vector<Flag> flags;
        IdentifierStyle identifierStyle;
        std::string iid;
        ImageExportMode imageExportMode;
        std::string jsonVersion;
        std::string levelNamePattern;
        std::vector<Level> levels;
        bool minifyJson;
        int64_t nextUid;
        std::optional<std::string> pngFilePattern;
        bool simplifiedExport;
        std::vector<LdtkTableOfContentEntry> toc;
        std::optional<std::string> tutorialDesc;
        std::optional<int64_t> worldGridHeight;
        std::optional<int64_t> worldGridWidth;
        std::optional<WorldLayout> worldLayout;
        std::vector<World> worlds;

        public:
        /**
         * This object is not actually used by LDtk. It ONLY exists to force explicit references to
         * all types, to make sure QuickType finds them and integrate all of them. Otherwise,
         * Quicktype will drop types that are not explicitely used.
         */
        std::optional<ForcedRefs> getForcedRefs() const { return forcedRefs; }
        void setForcedRefs(std::optional<ForcedRefs> value) { this->forcedRefs = value; }

        /**
         * LDtk application build identifier.<br/>  This is only used to identify the LDtk version
         * that generated this particular project file, which can be useful for specific bug fixing.
         * Note that the build identifier is just the date of the release, so it's not unique to
         * each user (one single global ID per LDtk public release), and as a result, completely
         * anonymous.
         */
        const double & getAppBuildId() const { return appBuildId; }
        double & getMutableAppBuildId() { return appBuildId; }
        void setAppBuildId(const double & value) { this->appBuildId = value; }

        /**
         * Number of backup files to keep, if the `backupOnSave` is TRUE
         */
        const int64_t & getBackupLimit() const { return backupLimit; }
        int64_t & getMutableBackupLimit() { return backupLimit; }
        void setBackupLimit(const int64_t & value) { this->backupLimit = value; }

        /**
         * If TRUE, an extra copy of the project will be created in a sub folder, when saving.
         */
        const bool & getBackupOnSave() const { return backupOnSave; }
        bool & getMutableBackupOnSave() { return backupOnSave; }
        void setBackupOnSave(const bool & value) { this->backupOnSave = value; }

        /**
         * Target relative path to store backup files
         */
        std::optional<std::string> getBackupRelPath() const { return backupRelPath; }
        void setBackupRelPath(std::optional<std::string> value) { this->backupRelPath = value; }

        /**
         * Project background color
         */
        const std::string & getBgColor() const { return bgColor; }
        std::string & getMutableBgColor() { return bgColor; }
        void setBgColor(const std::string & value) { this->bgColor = value; }

        /**
         * An array of command lines that can be ran manually by the user
         */
        const std::vector<LdtkCustomCommand> & getCustomCommands() const { return customCommands; }
        std::vector<LdtkCustomCommand> & getMutableCustomCommands() { return customCommands; }
        void setCustomCommands(const std::vector<LdtkCustomCommand> & value) { this->customCommands = value; }

        /**
         * Default height for new entities
         */
        const int64_t & getDefaultEntityHeight() const { return defaultEntityHeight; }
        int64_t & getMutableDefaultEntityHeight() { return defaultEntityHeight; }
        void setDefaultEntityHeight(const int64_t & value) { this->defaultEntityHeight = value; }

        /**
         * Default width for new entities
         */
        const int64_t & getDefaultEntityWidth() const { return defaultEntityWidth; }
        int64_t & getMutableDefaultEntityWidth() { return defaultEntityWidth; }
        void setDefaultEntityWidth(const int64_t & value) { this->defaultEntityWidth = value; }

        /**
         * Default grid size for new layers
         */
        const int64_t & getDefaultGridSize() const { return defaultGridSize; }
        int64_t & getMutableDefaultGridSize() { return defaultGridSize; }
        void setDefaultGridSize(const int64_t & value) { this->defaultGridSize = value; }

        /**
         * Default background color of levels
         */
        const std::string & getDefaultLevelBgColor() const { return defaultLevelBgColor; }
        std::string & getMutableDefaultLevelBgColor() { return defaultLevelBgColor; }
        void setDefaultLevelBgColor(const std::string & value) { this->defaultLevelBgColor = value; }

        /**
         * **WARNING**: this field will move to the `worlds` array after the "multi-worlds" update.
         * It will then be `null`. You can enable the Multi-worlds advanced project option to enable
         * the change immediately.<br/><br/>  Default new level height
         */
        std::optional<int64_t> getDefaultLevelHeight() const { return defaultLevelHeight; }
        void setDefaultLevelHeight(std::optional<int64_t> value) { this->defaultLevelHeight = value; }

        /**
         * **WARNING**: this field will move to the `worlds` array after the "multi-worlds" update.
         * It will then be `null`. You can enable the Multi-worlds advanced project option to enable
         * the change immediately.<br/><br/>  Default new level width
         */
        std::optional<int64_t> getDefaultLevelWidth() const { return defaultLevelWidth; }
        void setDefaultLevelWidth(std::optional<int64_t> value) { this->defaultLevelWidth = value; }

        /**
         * Default X pivot (0 to 1) for new entities
         */
        const double & getDefaultPivotX() const { return defaultPivotX; }
        double & getMutableDefaultPivotX() { return defaultPivotX; }
        void setDefaultPivotX(const double & value) { this->defaultPivotX = value; }

        /**
         * Default Y pivot (0 to 1) for new entities
         */
        const double & getDefaultPivotY() const { return defaultPivotY; }
        double & getMutableDefaultPivotY() { return defaultPivotY; }
        void setDefaultPivotY(const double & value) { this->defaultPivotY = value; }

        /**
         * A structure containing all the definitions of this project
         */
        const Definitions & getDefs() const { return defs; }
        Definitions & getMutableDefs() { return defs; }
        void setDefs(const Definitions & value) { this->defs = value; }

        /**
         * If the project isn't in MultiWorlds mode, this is the IID of the internal "dummy" World.
         */
        const std::string & getDummyWorldIid() const { return dummyWorldIid; }
        std::string & getMutableDummyWorldIid() { return dummyWorldIid; }
        void setDummyWorldIid(const std::string & value) { this->dummyWorldIid = value; }

        /**
         * If TRUE, the exported PNGs will include the level background (color or image).
         */
        const bool & getExportLevelBg() const { return exportLevelBg; }
        bool & getMutableExportLevelBg() { return exportLevelBg; }
        void setExportLevelBg(const bool & value) { this->exportLevelBg = value; }

        /**
         * **WARNING**: this deprecated value is no longer exported since version 0.9.3  Replaced
         * by: `imageExportMode`
         */
        std::optional<bool> getExportPng() const { return exportPng; }
        void setExportPng(std::optional<bool> value) { this->exportPng = value; }

        /**
         * If TRUE, a Tiled compatible file will also be generated along with the LDtk JSON file
         * (default is FALSE)
         */
        const bool & getExportTiled() const { return exportTiled; }
        bool & getMutableExportTiled() { return exportTiled; }
        void setExportTiled(const bool & value) { this->exportTiled = value; }

        /**
         * If TRUE, one file will be saved for the project (incl. all its definitions) and one file
         * in a sub-folder for each level.
         */
        const bool & getExternalLevels() const { return externalLevels; }
        bool & getMutableExternalLevels() { return externalLevels; }
        void setExternalLevels(const bool & value) { this->externalLevels = value; }

        /**
         * An array containing various advanced flags (ie. options or other states). Possible
         * values: `DiscardPreCsvIntGrid`, `ExportPreCsvIntGridFormat`, `IgnoreBackupSuggest`,
         * `PrependIndexToLevelFileNames`, `MultiWorlds`, `UseMultilinesType`
         */
        const std::vector<Flag> & getFlags() const { return flags; }
        std::vector<Flag> & getMutableFlags() { return flags; }
        void setFlags(const std::vector<Flag> & value) { this->flags = value; }

        /**
         * Naming convention for Identifiers (first-letter uppercase, full uppercase etc.) Possible
         * values: `Capitalize`, `Uppercase`, `Lowercase`, `Free`
         */
        const IdentifierStyle & getIdentifierStyle() const { return identifierStyle; }
        IdentifierStyle & getMutableIdentifierStyle() { return identifierStyle; }
        void setIdentifierStyle(const IdentifierStyle & value) { this->identifierStyle = value; }

        /**
         * Unique project identifier
         */
        const std::string & getIid() const { return iid; }
        std::string & getMutableIid() { return iid; }
        void setIid(const std::string & value) { this->iid = value; }

        /**
         * "Image export" option when saving project. Possible values: `None`, `OneImagePerLayer`,
         * `OneImagePerLevel`, `LayersAndLevels`
         */
        const ImageExportMode & getImageExportMode() const { return imageExportMode; }
        ImageExportMode & getMutableImageExportMode() { return imageExportMode; }
        void setImageExportMode(const ImageExportMode & value) { this->imageExportMode = value; }

        /**
         * File format version
         */
        const std::string & getJsonVersion() const { return jsonVersion; }
        std::string & getMutableJsonVersion() { return jsonVersion; }
        void setJsonVersion(const std::string & value) { this->jsonVersion = value; }

        /**
         * The default naming convention for level identifiers.
         */
        const std::string & getLevelNamePattern() const { return levelNamePattern; }
        std::string & getMutableLevelNamePattern() { return levelNamePattern; }
        void setLevelNamePattern(const std::string & value) { this->levelNamePattern = value; }

        /**
         * All levels. The order of this array is only relevant in `LinearHorizontal` and
         * `linearVertical` world layouts (see `worldLayout` value).<br/>  Otherwise, you should
         * refer to the `worldX`,`worldY` coordinates of each Level.
         */
        const std::vector<Level> & getLevels() const { return levels; }
        std::vector<Level> & getMutableLevels() { return levels; }
        void setLevels(const std::vector<Level> & value) { this->levels = value; }

        /**
         * If TRUE, the Json is partially minified (no indentation, nor line breaks, default is
         * FALSE)
         */
        const bool & getMinifyJson() const { return minifyJson; }
        bool & getMutableMinifyJson() { return minifyJson; }
        void setMinifyJson(const bool & value) { this->minifyJson = value; }

        /**
         * Next Unique integer ID available
         */
        const int64_t & getNextUid() const { return nextUid; }
        int64_t & getMutableNextUid() { return nextUid; }
        void setNextUid(const int64_t & value) { this->nextUid = value; }

        /**
         * File naming pattern for exported PNGs
         */
        std::optional<std::string> getPngFilePattern() const { return pngFilePattern; }
        void setPngFilePattern(std::optional<std::string> value) { this->pngFilePattern = value; }

        /**
         * If TRUE, a very simplified will be generated on saving, for quicker & easier engine
         * integration.
         */
        const bool & getSimplifiedExport() const { return simplifiedExport; }
        bool & getMutableSimplifiedExport() { return simplifiedExport; }
        void setSimplifiedExport(const bool & value) { this->simplifiedExport = value; }

        /**
         * All instances of entities that have their `exportToToc` flag enabled are listed in this
         * array.
         */
        const std::vector<LdtkTableOfContentEntry> & getToc() const { return toc; }
        std::vector<LdtkTableOfContentEntry> & getMutableToc() { return toc; }
        void setToc(const std::vector<LdtkTableOfContentEntry> & value) { this->toc = value; }

        /**
         * This optional description is used by LDtk Samples to show up some informations and
         * instructions.
         */
        std::optional<std::string> getTutorialDesc() const { return tutorialDesc; }
        void setTutorialDesc(std::optional<std::string> value) { this->tutorialDesc = value; }

        /**
         * **WARNING**: this field will move to the `worlds` array after the "multi-worlds" update.
         * It will then be `null`. You can enable the Multi-worlds advanced project option to enable
         * the change immediately.<br/><br/>  Height of the world grid in pixels.
         */
        std::optional<int64_t> getWorldGridHeight() const { return worldGridHeight; }
        void setWorldGridHeight(std::optional<int64_t> value) { this->worldGridHeight = value; }

        /**
         * **WARNING**: this field will move to the `worlds` array after the "multi-worlds" update.
         * It will then be `null`. You can enable the Multi-worlds advanced project option to enable
         * the change immediately.<br/><br/>  Width of the world grid in pixels.
         */
        std::optional<int64_t> getWorldGridWidth() const { return worldGridWidth; }
        void setWorldGridWidth(std::optional<int64_t> value) { this->worldGridWidth = value; }

        /**
         * **WARNING**: this field will move to the `worlds` array after the "multi-worlds" update.
         * It will then be `null`. You can enable the Multi-worlds advanced project option to enable
         * the change immediately.<br/><br/>  An enum that describes how levels are organized in
         * this project (ie. linearly or in a 2D space). Possible values: &lt;`null`&gt;, `Free`,
         * `GridVania`, `LinearHorizontal`, `LinearVertical`
         */
        std::optional<WorldLayout> getWorldLayout() const { return worldLayout; }
        void setWorldLayout(std::optional<WorldLayout> value) { this->worldLayout = value; }

        /**
         * This array will be empty, unless you enable the Multi-Worlds in the project advanced
         * settings.<br/><br/> - in current version, a LDtk project file can only contain a single
         * world with multiple levels in it. In this case, levels and world layout related settings
         * are stored in the root of the JSON.<br/> - with "Multi-worlds" enabled, there will be a
         * `worlds` array in root, each world containing levels and layout settings. Basically, it's
         * pretty much only about moving the `levels` array to the `worlds` array, along with world
         * layout related values (eg. `worldGridWidth` etc).<br/><br/>If you want to start
         * supporting this future update easily, please refer to this documentation:
         * https://github.com/deepnight/ldtk/issues/231
         */
        const std::vector<World> & getWorlds() const { return worlds; }
        std::vector<World> & getMutableWorlds() { return worlds; }
        void setWorlds(const std::vector<World> & value) { this->worlds = value; }
    };
}

namespace ldtk {
    void from_json(const json & j, LdtkCustomCommand & x);
    void to_json(json & j, const LdtkCustomCommand & x);

    void from_json(const json & j, FieldDefinition & x);
    void to_json(json & j, const FieldDefinition & x);

    void from_json(const json & j, TilesetRectangle & x);
    void to_json(json & j, const TilesetRectangle & x);

    void from_json(const json & j, EntityDefinition & x);
    void to_json(json & j, const EntityDefinition & x);

    void from_json(const json & j, EnumValueDefinition & x);
    void to_json(json & j, const EnumValueDefinition & x);

    void from_json(const json & j, EnumDefinition & x);
    void to_json(json & j, const EnumDefinition & x);

    void from_json(const json & j, AutoLayerRuleDefinition & x);
    void to_json(json & j, const AutoLayerRuleDefinition & x);

    void from_json(const json & j, AutoLayerRuleGroup & x);
    void to_json(json & j, const AutoLayerRuleGroup & x);

    void from_json(const json & j, IntGridValueDefinition & x);
    void to_json(json & j, const IntGridValueDefinition & x);

    void from_json(const json & j, IntGridValueGroupDefinition & x);
    void to_json(json & j, const IntGridValueGroupDefinition & x);

    void from_json(const json & j, LayerDefinition & x);
    void to_json(json & j, const LayerDefinition & x);

    void from_json(const json & j, TileCustomMetadata & x);
    void to_json(json & j, const TileCustomMetadata & x);

    void from_json(const json & j, EnumTagValue & x);
    void to_json(json & j, const EnumTagValue & x);

    void from_json(const json & j, TilesetDefinition & x);
    void to_json(json & j, const TilesetDefinition & x);

    void from_json(const json & j, Definitions & x);
    void to_json(json & j, const Definitions & x);

    void from_json(const json & j, FieldInstance & x);
    void to_json(json & j, const FieldInstance & x);

    void from_json(const json & j, EntityInstance & x);
    void to_json(json & j, const EntityInstance & x);

    void from_json(const json & j, ReferenceToAnEntityInstance & x);
    void to_json(json & j, const ReferenceToAnEntityInstance & x);

    void from_json(const json & j, GridPoint & x);
    void to_json(json & j, const GridPoint & x);

    void from_json(const json & j, IntGridValueInstance & x);
    void to_json(json & j, const IntGridValueInstance & x);

    void from_json(const json & j, TileInstance & x);
    void to_json(json & j, const TileInstance & x);

    void from_json(const json & j, LayerInstance & x);
    void to_json(json & j, const LayerInstance & x);

    void from_json(const json & j, LevelBackgroundPosition & x);
    void to_json(json & j, const LevelBackgroundPosition & x);

    void from_json(const json & j, NeighbourLevel & x);
    void to_json(json & j, const NeighbourLevel & x);

    void from_json(const json & j, Level & x);
    void to_json(json & j, const Level & x);

    void from_json(const json & j, LdtkTableOfContentEntry & x);
    void to_json(json & j, const LdtkTableOfContentEntry & x);

    void from_json(const json & j, World & x);
    void to_json(json & j, const World & x);

    void from_json(const json & j, ForcedRefs & x);
    void to_json(json & j, const ForcedRefs & x);

    void from_json(const json & j, Coordinate & x);
    void to_json(json & j, const Coordinate & x);

    void from_json(const json & j, When & x);
    void to_json(json & j, const When & x);

    void from_json(const json & j, AllowedRefs & x);
    void to_json(json & j, const AllowedRefs & x);

    void from_json(const json & j, EditorDisplayMode & x);
    void to_json(json & j, const EditorDisplayMode & x);

    void from_json(const json & j, EditorDisplayPos & x);
    void to_json(json & j, const EditorDisplayPos & x);

    void from_json(const json & j, EditorLinkStyle & x);
    void to_json(json & j, const EditorLinkStyle & x);

    void from_json(const json & j, TextLanguageMode & x);
    void to_json(json & j, const TextLanguageMode & x);

    void from_json(const json & j, LimitBehavior & x);
    void to_json(json & j, const LimitBehavior & x);

    void from_json(const json & j, LimitScope & x);
    void to_json(json & j, const LimitScope & x);

    void from_json(const json & j, RenderMode & x);
    void to_json(json & j, const RenderMode & x);

    void from_json(const json & j, TileRenderMode & x);
    void to_json(json & j, const TileRenderMode & x);

    void from_json(const json & j, Checker & x);
    void to_json(json & j, const Checker & x);

    void from_json(const json & j, TileMode & x);
    void to_json(json & j, const TileMode & x);

    void from_json(const json & j, Type & x);
    void to_json(json & j, const Type & x);

    void from_json(const json & j, EmbedAtlas & x);
    void to_json(json & j, const EmbedAtlas & x);

    void from_json(const json & j, Flag & x);
    void to_json(json & j, const Flag & x);

    void from_json(const json & j, BgPos & x);
    void to_json(json & j, const BgPos & x);

    void from_json(const json & j, WorldLayout & x);
    void to_json(json & j, const WorldLayout & x);

    void from_json(const json & j, IdentifierStyle & x);
    void to_json(json & j, const IdentifierStyle & x);

    void from_json(const json & j, ImageExportMode & x);
    void to_json(json & j, const ImageExportMode & x);

    inline void from_json(const json & j, LdtkCustomCommand& x) {
        x.setCommand(j.at("command").get<std::string>());
        x.setWhen(j.at("when").get<When>());
    }

    inline void to_json(json & j, const LdtkCustomCommand & x) {
        j = json::object();
        j["command"] = x.getCommand();
        j["when"] = x.getWhen();
    }

    inline void from_json(const json & j, FieldDefinition& x) {
        x.setType(j.at("__type").get<std::string>());
        x.setAcceptFileTypes(get_stack_optional<std::vector<std::string>>(j, "acceptFileTypes"));
        x.setAllowedRefs(j.at("allowedRefs").get<AllowedRefs>());
        x.setAllowedRefsEntityUid(get_stack_optional<int64_t>(j, "allowedRefsEntityUid"));
        x.setAllowedRefTags(j.at("allowedRefTags").get<std::vector<std::string>>());
        x.setAllowOutOfLevelRef(j.at("allowOutOfLevelRef").get<bool>());
        x.setArrayMaxLength(get_stack_optional<int64_t>(j, "arrayMaxLength"));
        x.setArrayMinLength(get_stack_optional<int64_t>(j, "arrayMinLength"));
        x.setAutoChainRef(j.at("autoChainRef").get<bool>());
        x.setCanBeNull(j.at("canBeNull").get<bool>());
        x.setDefaultOverride(get_untyped(j, "defaultOverride"));
        x.setDoc(get_stack_optional<std::string>(j, "doc"));
        x.setEditorAlwaysShow(j.at("editorAlwaysShow").get<bool>());
        x.setEditorCutLongValues(j.at("editorCutLongValues").get<bool>());
        x.setEditorDisplayColor(get_stack_optional<std::string>(j, "editorDisplayColor"));
        x.setEditorDisplayMode(j.at("editorDisplayMode").get<EditorDisplayMode>());
        x.setEditorDisplayPos(j.at("editorDisplayPos").get<EditorDisplayPos>());
        x.setEditorDisplayScale(j.at("editorDisplayScale").get<double>());
        x.setEditorLinkStyle(j.at("editorLinkStyle").get<EditorLinkStyle>());
        x.setEditorShowInWorld(j.at("editorShowInWorld").get<bool>());
        x.setEditorTextPrefix(get_stack_optional<std::string>(j, "editorTextPrefix"));
        x.setEditorTextSuffix(get_stack_optional<std::string>(j, "editorTextSuffix"));
        x.setIdentifier(j.at("identifier").get<std::string>());
        x.setIsArray(j.at("isArray").get<bool>());
        x.setMax(get_stack_optional<double>(j, "max"));
        x.setMin(get_stack_optional<double>(j, "min"));
        x.setRegex(get_stack_optional<std::string>(j, "regex"));
        x.setSymmetricalRef(j.at("symmetricalRef").get<bool>());
        x.setTextLanguageMode(get_stack_optional<TextLanguageMode>(j, "textLanguageMode"));
        x.setTilesetUid(get_stack_optional<int64_t>(j, "tilesetUid"));
        x.setFieldDefinitionType(j.at("type").get<std::string>());
        x.setUid(j.at("uid").get<int64_t>());
        x.setUseForSmartColor(j.at("useForSmartColor").get<bool>());
    }

    inline void to_json(json & j, const FieldDefinition & x) {
        j = json::object();
        j["__type"] = x.getType();
        j["acceptFileTypes"] = x.getAcceptFileTypes();
        j["allowedRefs"] = x.getAllowedRefs();
        j["allowedRefsEntityUid"] = x.getAllowedRefsEntityUid();
        j["allowedRefTags"] = x.getAllowedRefTags();
        j["allowOutOfLevelRef"] = x.getAllowOutOfLevelRef();
        j["arrayMaxLength"] = x.getArrayMaxLength();
        j["arrayMinLength"] = x.getArrayMinLength();
        j["autoChainRef"] = x.getAutoChainRef();
        j["canBeNull"] = x.getCanBeNull();
        j["defaultOverride"] = x.getDefaultOverride();
        j["doc"] = x.getDoc();
        j["editorAlwaysShow"] = x.getEditorAlwaysShow();
        j["editorCutLongValues"] = x.getEditorCutLongValues();
        j["editorDisplayColor"] = x.getEditorDisplayColor();
        j["editorDisplayMode"] = x.getEditorDisplayMode();
        j["editorDisplayPos"] = x.getEditorDisplayPos();
        j["editorDisplayScale"] = x.getEditorDisplayScale();
        j["editorLinkStyle"] = x.getEditorLinkStyle();
        j["editorShowInWorld"] = x.getEditorShowInWorld();
        j["editorTextPrefix"] = x.getEditorTextPrefix();
        j["editorTextSuffix"] = x.getEditorTextSuffix();
        j["identifier"] = x.getIdentifier();
        j["isArray"] = x.getIsArray();
        j["max"] = x.getMax();
        j["min"] = x.getMin();
        j["regex"] = x.getRegex();
        j["symmetricalRef"] = x.getSymmetricalRef();
        j["textLanguageMode"] = x.getTextLanguageMode();
        j["tilesetUid"] = x.getTilesetUid();
        j["type"] = x.getFieldDefinitionType();
        j["uid"] = x.getUid();
        j["useForSmartColor"] = x.getUseForSmartColor();
    }

    inline void from_json(const json & j, TilesetRectangle& x) {
        x.setH(j.at("h").get<int64_t>());
        x.setTilesetUid(j.at("tilesetUid").get<int64_t>());
        x.setW(j.at("w").get<int64_t>());
        x.setX(j.at("x").get<int64_t>());
        x.setY(j.at("y").get<int64_t>());
    }

    inline void to_json(json & j, const TilesetRectangle & x) {
        j = json::object();
        j["h"] = x.getH();
        j["tilesetUid"] = x.getTilesetUid();
        j["w"] = x.getW();
        j["x"] = x.getX();
        j["y"] = x.getY();
    }

    inline void from_json(const json & j, EntityDefinition& x) {
        x.setColor(j.at("color").get<std::string>());
        x.setDoc(get_stack_optional<std::string>(j, "doc"));
        x.setExportToToc(j.at("exportToToc").get<bool>());
        x.setFieldDefs(j.at("fieldDefs").get<std::vector<FieldDefinition>>());
        x.setFillOpacity(j.at("fillOpacity").get<double>());
        x.setHeight(j.at("height").get<int64_t>());
        x.setHollow(j.at("hollow").get<bool>());
        x.setIdentifier(j.at("identifier").get<std::string>());
        x.setKeepAspectRatio(j.at("keepAspectRatio").get<bool>());
        x.setLimitBehavior(j.at("limitBehavior").get<LimitBehavior>());
        x.setLimitScope(j.at("limitScope").get<LimitScope>());
        x.setLineOpacity(j.at("lineOpacity").get<double>());
        x.setMaxCount(j.at("maxCount").get<int64_t>());
        x.setMaxHeight(get_stack_optional<int64_t>(j, "maxHeight"));
        x.setMaxWidth(get_stack_optional<int64_t>(j, "maxWidth"));
        x.setMinHeight(get_stack_optional<int64_t>(j, "minHeight"));
        x.setMinWidth(get_stack_optional<int64_t>(j, "minWidth"));
        x.setNineSliceBorders(j.at("nineSliceBorders").get<std::vector<int64_t>>());
        x.setPivotX(j.at("pivotX").get<double>());
        x.setPivotY(j.at("pivotY").get<double>());
        x.setRenderMode(j.at("renderMode").get<RenderMode>());
        x.setResizableX(j.at("resizableX").get<bool>());
        x.setResizableY(j.at("resizableY").get<bool>());
        x.setShowName(j.at("showName").get<bool>());
        x.setTags(j.at("tags").get<std::vector<std::string>>());
        x.setTileId(get_stack_optional<int64_t>(j, "tileId"));
        x.setTileOpacity(j.at("tileOpacity").get<double>());
        x.setTileRect(get_stack_optional<TilesetRectangle>(j, "tileRect"));
        x.setTileRenderMode(j.at("tileRenderMode").get<TileRenderMode>());
        x.setTilesetId(get_stack_optional<int64_t>(j, "tilesetId"));
        x.setUid(j.at("uid").get<int64_t>());
        x.setUiTileRect(get_stack_optional<TilesetRectangle>(j, "uiTileRect"));
        x.setWidth(j.at("width").get<int64_t>());
    }

    inline void to_json(json & j, const EntityDefinition & x) {
        j = json::object();
        j["color"] = x.getColor();
        j["doc"] = x.getDoc();
        j["exportToToc"] = x.getExportToToc();
        j["fieldDefs"] = x.getFieldDefs();
        j["fillOpacity"] = x.getFillOpacity();
        j["height"] = x.getHeight();
        j["hollow"] = x.getHollow();
        j["identifier"] = x.getIdentifier();
        j["keepAspectRatio"] = x.getKeepAspectRatio();
        j["limitBehavior"] = x.getLimitBehavior();
        j["limitScope"] = x.getLimitScope();
        j["lineOpacity"] = x.getLineOpacity();
        j["maxCount"] = x.getMaxCount();
        j["maxHeight"] = x.getMaxHeight();
        j["maxWidth"] = x.getMaxWidth();
        j["minHeight"] = x.getMinHeight();
        j["minWidth"] = x.getMinWidth();
        j["nineSliceBorders"] = x.getNineSliceBorders();
        j["pivotX"] = x.getPivotX();
        j["pivotY"] = x.getPivotY();
        j["renderMode"] = x.getRenderMode();
        j["resizableX"] = x.getResizableX();
        j["resizableY"] = x.getResizableY();
        j["showName"] = x.getShowName();
        j["tags"] = x.getTags();
        j["tileId"] = x.getTileId();
        j["tileOpacity"] = x.getTileOpacity();
        j["tileRect"] = x.getTileRect();
        j["tileRenderMode"] = x.getTileRenderMode();
        j["tilesetId"] = x.getTilesetId();
        j["uid"] = x.getUid();
        j["uiTileRect"] = x.getUiTileRect();
        j["width"] = x.getWidth();
    }

    inline void from_json(const json & j, EnumValueDefinition& x) {
        x.setTileSrcRect(get_stack_optional<std::vector<int64_t>>(j, "__tileSrcRect"));
        x.setColor(j.at("color").get<int64_t>());
        x.setId(j.at("id").get<std::string>());
        x.setTileId(get_stack_optional<int64_t>(j, "tileId"));
        x.setTileRect(get_stack_optional<TilesetRectangle>(j, "tileRect"));
    }

    inline void to_json(json & j, const EnumValueDefinition & x) {
        j = json::object();
        j["__tileSrcRect"] = x.getTileSrcRect();
        j["color"] = x.getColor();
        j["id"] = x.getId();
        j["tileId"] = x.getTileId();
        j["tileRect"] = x.getTileRect();
    }

    inline void from_json(const json & j, EnumDefinition& x) {
        x.setExternalFileChecksum(get_stack_optional<std::string>(j, "externalFileChecksum"));
        x.setExternalRelPath(get_stack_optional<std::string>(j, "externalRelPath"));
        x.setIconTilesetUid(get_stack_optional<int64_t>(j, "iconTilesetUid"));
        x.setIdentifier(j.at("identifier").get<std::string>());
        x.setTags(j.at("tags").get<std::vector<std::string>>());
        x.setUid(j.at("uid").get<int64_t>());
        x.setValues(j.at("values").get<std::vector<EnumValueDefinition>>());
    }

    inline void to_json(json & j, const EnumDefinition & x) {
        j = json::object();
        j["externalFileChecksum"] = x.getExternalFileChecksum();
        j["externalRelPath"] = x.getExternalRelPath();
        j["iconTilesetUid"] = x.getIconTilesetUid();
        j["identifier"] = x.getIdentifier();
        j["tags"] = x.getTags();
        j["uid"] = x.getUid();
        j["values"] = x.getValues();
    }

    inline void from_json(const json & j, AutoLayerRuleDefinition& x) {
        x.setActive(j.at("active").get<bool>());
        x.setAlpha(j.at("alpha").get<double>());
        x.setBreakOnMatch(j.at("breakOnMatch").get<bool>());
        x.setChance(j.at("chance").get<double>());
        x.setChecker(j.at("checker").get<Checker>());
        x.setFlipX(j.at("flipX").get<bool>());
        x.setFlipY(j.at("flipY").get<bool>());
        x.setOutOfBoundsValue(get_stack_optional<int64_t>(j, "outOfBoundsValue"));
        x.setPattern(j.at("pattern").get<std::vector<int64_t>>());
        x.setPerlinActive(j.at("perlinActive").get<bool>());
        x.setPerlinOctaves(j.at("perlinOctaves").get<double>());
        x.setPerlinScale(j.at("perlinScale").get<double>());
        x.setPerlinSeed(j.at("perlinSeed").get<double>());
        x.setPivotX(j.at("pivotX").get<double>());
        x.setPivotY(j.at("pivotY").get<double>());
        x.setSize(j.at("size").get<int64_t>());
        x.setTileIds(j.at("tileIds").get<std::vector<int64_t>>());
        x.setTileMode(j.at("tileMode").get<TileMode>());
        x.setTileRandomXMax(j.at("tileRandomXMax").get<int64_t>());
        x.setTileRandomXMin(j.at("tileRandomXMin").get<int64_t>());
        x.setTileRandomYMax(j.at("tileRandomYMax").get<int64_t>());
        x.setTileRandomYMin(j.at("tileRandomYMin").get<int64_t>());
        x.setTileXOffset(j.at("tileXOffset").get<int64_t>());
        x.setTileYOffset(j.at("tileYOffset").get<int64_t>());
        x.setUid(j.at("uid").get<int64_t>());
        x.setXModulo(j.at("xModulo").get<int64_t>());
        x.setXOffset(j.at("xOffset").get<int64_t>());
        x.setYModulo(j.at("yModulo").get<int64_t>());
        x.setYOffset(j.at("yOffset").get<int64_t>());
    }

    inline void to_json(json & j, const AutoLayerRuleDefinition & x) {
        j = json::object();
        j["active"] = x.getActive();
        j["alpha"] = x.getAlpha();
        j["breakOnMatch"] = x.getBreakOnMatch();
        j["chance"] = x.getChance();
        j["checker"] = x.getChecker();
        j["flipX"] = x.getFlipX();
        j["flipY"] = x.getFlipY();
        j["outOfBoundsValue"] = x.getOutOfBoundsValue();
        j["pattern"] = x.getPattern();
        j["perlinActive"] = x.getPerlinActive();
        j["perlinOctaves"] = x.getPerlinOctaves();
        j["perlinScale"] = x.getPerlinScale();
        j["perlinSeed"] = x.getPerlinSeed();
        j["pivotX"] = x.getPivotX();
        j["pivotY"] = x.getPivotY();
        j["size"] = x.getSize();
        j["tileIds"] = x.getTileIds();
        j["tileMode"] = x.getTileMode();
        j["tileRandomXMax"] = x.getTileRandomXMax();
        j["tileRandomXMin"] = x.getTileRandomXMin();
        j["tileRandomYMax"] = x.getTileRandomYMax();
        j["tileRandomYMin"] = x.getTileRandomYMin();
        j["tileXOffset"] = x.getTileXOffset();
        j["tileYOffset"] = x.getTileYOffset();
        j["uid"] = x.getUid();
        j["xModulo"] = x.getXModulo();
        j["xOffset"] = x.getXOffset();
        j["yModulo"] = x.getYModulo();
        j["yOffset"] = x.getYOffset();
    }

    inline void from_json(const json & j, AutoLayerRuleGroup& x) {
        x.setActive(j.at("active").get<bool>());
        x.setCollapsed(get_stack_optional<bool>(j, "collapsed"));
        x.setColor(get_stack_optional<std::string>(j, "color"));
        x.setIcon(get_stack_optional<TilesetRectangle>(j, "icon"));
        x.setIsOptional(j.at("isOptional").get<bool>());
        x.setName(j.at("name").get<std::string>());
        x.setRules(j.at("rules").get<std::vector<AutoLayerRuleDefinition>>());
        x.setUid(j.at("uid").get<int64_t>());
        x.setUsesWizard(j.at("usesWizard").get<bool>());
    }

    inline void to_json(json & j, const AutoLayerRuleGroup & x) {
        j = json::object();
        j["active"] = x.getActive();
        j["collapsed"] = x.getCollapsed();
        j["color"] = x.getColor();
        j["icon"] = x.getIcon();
        j["isOptional"] = x.getIsOptional();
        j["name"] = x.getName();
        j["rules"] = x.getRules();
        j["uid"] = x.getUid();
        j["usesWizard"] = x.getUsesWizard();
    }

    inline void from_json(const json & j, IntGridValueDefinition& x) {
        x.setColor(j.at("color").get<std::string>());
        x.setGroupUid(j.at("groupUid").get<int64_t>());
        x.setIdentifier(get_stack_optional<std::string>(j, "identifier"));
        x.setTile(get_stack_optional<TilesetRectangle>(j, "tile"));
        x.setValue(j.at("value").get<int64_t>());
    }

    inline void to_json(json & j, const IntGridValueDefinition & x) {
        j = json::object();
        j["color"] = x.getColor();
        j["groupUid"] = x.getGroupUid();
        j["identifier"] = x.getIdentifier();
        j["tile"] = x.getTile();
        j["value"] = x.getValue();
    }

    inline void from_json(const json & j, IntGridValueGroupDefinition& x) {
        x.setColor(get_stack_optional<std::string>(j, "color"));
        x.setIdentifier(get_stack_optional<std::string>(j, "identifier"));
        x.setUid(j.at("uid").get<int64_t>());
    }

    inline void to_json(json & j, const IntGridValueGroupDefinition & x) {
        j = json::object();
        j["color"] = x.getColor();
        j["identifier"] = x.getIdentifier();
        j["uid"] = x.getUid();
    }

    inline void from_json(const json & j, LayerDefinition& x) {
        x.setType(j.at("__type").get<std::string>());
        x.setAutoRuleGroups(j.at("autoRuleGroups").get<std::vector<AutoLayerRuleGroup>>());
        x.setAutoSourceLayerDefUid(get_stack_optional<int64_t>(j, "autoSourceLayerDefUid"));
        x.setAutoTilesetDefUid(get_stack_optional<int64_t>(j, "autoTilesetDefUid"));
        x.setCanSelectWhenInactive(j.at("canSelectWhenInactive").get<bool>());
        x.setDisplayOpacity(j.at("displayOpacity").get<double>());
        x.setDoc(get_stack_optional<std::string>(j, "doc"));
        x.setExcludedTags(j.at("excludedTags").get<std::vector<std::string>>());
        x.setGridSize(j.at("gridSize").get<int64_t>());
        x.setGuideGridHei(j.at("guideGridHei").get<int64_t>());
        x.setGuideGridWid(j.at("guideGridWid").get<int64_t>());
        x.setHideFieldsWhenInactive(j.at("hideFieldsWhenInactive").get<bool>());
        x.setHideInList(j.at("hideInList").get<bool>());
        x.setIdentifier(j.at("identifier").get<std::string>());
        x.setInactiveOpacity(j.at("inactiveOpacity").get<double>());
        x.setIntGridValues(j.at("intGridValues").get<std::vector<IntGridValueDefinition>>());
        x.setIntGridValuesGroups(j.at("intGridValuesGroups").get<std::vector<IntGridValueGroupDefinition>>());
        x.setParallaxFactorX(j.at("parallaxFactorX").get<double>());
        x.setParallaxFactorY(j.at("parallaxFactorY").get<double>());
        x.setParallaxScaling(j.at("parallaxScaling").get<bool>());
        x.setPxOffsetX(j.at("pxOffsetX").get<int64_t>());
        x.setPxOffsetY(j.at("pxOffsetY").get<int64_t>());
        x.setRenderInWorldView(j.at("renderInWorldView").get<bool>());
        x.setRequiredTags(j.at("requiredTags").get<std::vector<std::string>>());
        x.setTilePivotX(j.at("tilePivotX").get<double>());
        x.setTilePivotY(j.at("tilePivotY").get<double>());
        x.setTilesetDefUid(get_stack_optional<int64_t>(j, "tilesetDefUid"));
        x.setLayerDefinitionType(j.at("type").get<Type>());
        x.setUiColor(get_stack_optional<std::string>(j, "uiColor"));
        x.setUid(j.at("uid").get<int64_t>());
    }

    inline void to_json(json & j, const LayerDefinition & x) {
        j = json::object();
        j["__type"] = x.getType();
        j["autoRuleGroups"] = x.getAutoRuleGroups();
        j["autoSourceLayerDefUid"] = x.getAutoSourceLayerDefUid();
        j["autoTilesetDefUid"] = x.getAutoTilesetDefUid();
        j["canSelectWhenInactive"] = x.getCanSelectWhenInactive();
        j["displayOpacity"] = x.getDisplayOpacity();
        j["doc"] = x.getDoc();
        j["excludedTags"] = x.getExcludedTags();
        j["gridSize"] = x.getGridSize();
        j["guideGridHei"] = x.getGuideGridHei();
        j["guideGridWid"] = x.getGuideGridWid();
        j["hideFieldsWhenInactive"] = x.getHideFieldsWhenInactive();
        j["hideInList"] = x.getHideInList();
        j["identifier"] = x.getIdentifier();
        j["inactiveOpacity"] = x.getInactiveOpacity();
        j["intGridValues"] = x.getIntGridValues();
        j["intGridValuesGroups"] = x.getIntGridValuesGroups();
        j["parallaxFactorX"] = x.getParallaxFactorX();
        j["parallaxFactorY"] = x.getParallaxFactorY();
        j["parallaxScaling"] = x.getParallaxScaling();
        j["pxOffsetX"] = x.getPxOffsetX();
        j["pxOffsetY"] = x.getPxOffsetY();
        j["renderInWorldView"] = x.getRenderInWorldView();
        j["requiredTags"] = x.getRequiredTags();
        j["tilePivotX"] = x.getTilePivotX();
        j["tilePivotY"] = x.getTilePivotY();
        j["tilesetDefUid"] = x.getTilesetDefUid();
        j["type"] = x.getLayerDefinitionType();
        j["uiColor"] = x.getUiColor();
        j["uid"] = x.getUid();
    }

    inline void from_json(const json & j, TileCustomMetadata& x) {
        x.setData(j.at("data").get<std::string>());
        x.setTileId(j.at("tileId").get<int64_t>());
    }

    inline void to_json(json & j, const TileCustomMetadata & x) {
        j = json::object();
        j["data"] = x.getData();
        j["tileId"] = x.getTileId();
    }

    inline void from_json(const json & j, EnumTagValue& x) {
        x.setEnumValueId(j.at("enumValueId").get<std::string>());
        x.setTileIds(j.at("tileIds").get<std::vector<int64_t>>());
    }

    inline void to_json(json & j, const EnumTagValue & x) {
        j = json::object();
        j["enumValueId"] = x.getEnumValueId();
        j["tileIds"] = x.getTileIds();
    }

    inline void from_json(const json & j, TilesetDefinition& x) {
        x.setCHei(j.at("__cHei").get<int64_t>());
        x.setCWid(j.at("__cWid").get<int64_t>());
        x.setCachedPixelData(get_stack_optional<std::map<std::string, nlohmann::json>>(j, "cachedPixelData"));
        x.setCustomData(j.at("customData").get<std::vector<TileCustomMetadata>>());
        x.setEmbedAtlas(get_stack_optional<EmbedAtlas>(j, "embedAtlas"));
        x.setEnumTags(j.at("enumTags").get<std::vector<EnumTagValue>>());
        x.setIdentifier(j.at("identifier").get<std::string>());
        x.setPadding(j.at("padding").get<int64_t>());
        x.setPxHei(j.at("pxHei").get<int64_t>());
        x.setPxWid(j.at("pxWid").get<int64_t>());
        x.setRelPath(get_stack_optional<std::string>(j, "relPath"));
        x.setSavedSelections(j.at("savedSelections").get<std::vector<std::map<std::string, nlohmann::json>>>());
        x.setSpacing(j.at("spacing").get<int64_t>());
        x.setTags(j.at("tags").get<std::vector<std::string>>());
        x.setTagsSourceEnumUid(get_stack_optional<int64_t>(j, "tagsSourceEnumUid"));
        x.setTileGridSize(j.at("tileGridSize").get<int64_t>());
        x.setUid(j.at("uid").get<int64_t>());
    }

    inline void to_json(json & j, const TilesetDefinition & x) {
        j = json::object();
        j["__cHei"] = x.getCHei();
        j["__cWid"] = x.getCWid();
        j["cachedPixelData"] = x.getCachedPixelData();
        j["customData"] = x.getCustomData();
        j["embedAtlas"] = x.getEmbedAtlas();
        j["enumTags"] = x.getEnumTags();
        j["identifier"] = x.getIdentifier();
        j["padding"] = x.getPadding();
        j["pxHei"] = x.getPxHei();
        j["pxWid"] = x.getPxWid();
        j["relPath"] = x.getRelPath();
        j["savedSelections"] = x.getSavedSelections();
        j["spacing"] = x.getSpacing();
        j["tags"] = x.getTags();
        j["tagsSourceEnumUid"] = x.getTagsSourceEnumUid();
        j["tileGridSize"] = x.getTileGridSize();
        j["uid"] = x.getUid();
    }

    inline void from_json(const json & j, Definitions& x) {
        x.setEntities(j.at("entities").get<std::vector<EntityDefinition>>());
        x.setEnums(j.at("enums").get<std::vector<EnumDefinition>>());
        x.setExternalEnums(j.at("externalEnums").get<std::vector<EnumDefinition>>());
        x.setLayers(j.at("layers").get<std::vector<LayerDefinition>>());
        x.setLevelFields(j.at("levelFields").get<std::vector<FieldDefinition>>());
        x.setTilesets(j.at("tilesets").get<std::vector<TilesetDefinition>>());
    }

    inline void to_json(json & j, const Definitions & x) {
        j = json::object();
        j["entities"] = x.getEntities();
        j["enums"] = x.getEnums();
        j["externalEnums"] = x.getExternalEnums();
        j["layers"] = x.getLayers();
        j["levelFields"] = x.getLevelFields();
        j["tilesets"] = x.getTilesets();
    }

    inline void from_json(const json & j, FieldInstance& x) {
        x.setIdentifier(j.at("__identifier").get<std::string>());
        x.setTile(get_stack_optional<TilesetRectangle>(j, "__tile"));
        x.setType(j.at("__type").get<std::string>());
        x.setValue(get_untyped(j, "__value"));
        x.setDefUid(j.at("defUid").get<int64_t>());
        x.setRealEditorValues(j.at("realEditorValues").get<std::vector<nlohmann::json>>());
    }

    inline void to_json(json & j, const FieldInstance & x) {
        j = json::object();
        j["__identifier"] = x.getIdentifier();
        j["__tile"] = x.getTile();
        j["__type"] = x.getType();
        j["__value"] = x.getValue();
        j["defUid"] = x.getDefUid();
        j["realEditorValues"] = x.getRealEditorValues();
    }

    inline void from_json(const json & j, EntityInstance& x) {
        x.setGrid(j.at("__grid").get<std::vector<int64_t>>());
        x.setIdentifier(j.at("__identifier").get<std::string>());
        x.setPivot(j.at("__pivot").get<std::vector<double>>());
        x.setSmartColor(j.at("__smartColor").get<std::string>());
        x.setTags(j.at("__tags").get<std::vector<std::string>>());
        x.setTile(get_stack_optional<TilesetRectangle>(j, "__tile"));
        x.setWorldX(j.at("__worldX").get<int64_t>());
        x.setWorldY(j.at("__worldY").get<int64_t>());
        x.setDefUid(j.at("defUid").get<int64_t>());
        x.setFieldInstances(j.at("fieldInstances").get<std::vector<FieldInstance>>());
        x.setHeight(j.at("height").get<int64_t>());
        x.setIid(j.at("iid").get<std::string>());
        x.setPx(j.at("px").get<std::vector<int64_t>>());
        x.setWidth(j.at("width").get<int64_t>());
    }

    inline void to_json(json & j, const EntityInstance & x) {
        j = json::object();
        j["__grid"] = x.getGrid();
        j["__identifier"] = x.getIdentifier();
        j["__pivot"] = x.getPivot();
        j["__smartColor"] = x.getSmartColor();
        j["__tags"] = x.getTags();
        j["__tile"] = x.getTile();
        j["__worldX"] = x.getWorldX();
        j["__worldY"] = x.getWorldY();
        j["defUid"] = x.getDefUid();
        j["fieldInstances"] = x.getFieldInstances();
        j["height"] = x.getHeight();
        j["iid"] = x.getIid();
        j["px"] = x.getPx();
        j["width"] = x.getWidth();
    }

    inline void from_json(const json & j, ReferenceToAnEntityInstance& x) {
        x.setEntityIid(j.at("entityIid").get<std::string>());
        x.setLayerIid(j.at("layerIid").get<std::string>());
        x.setLevelIid(j.at("levelIid").get<std::string>());
        x.setWorldIid(j.at("worldIid").get<std::string>());
    }

    inline void to_json(json & j, const ReferenceToAnEntityInstance & x) {
        j = json::object();
        j["entityIid"] = x.getEntityIid();
        j["layerIid"] = x.getLayerIid();
        j["levelIid"] = x.getLevelIid();
        j["worldIid"] = x.getWorldIid();
    }

    inline void from_json(const json & j, GridPoint& x) {
        x.setCx(j.at("cx").get<int64_t>());
        x.setCy(j.at("cy").get<int64_t>());
    }

    inline void to_json(json & j, const GridPoint & x) {
        j = json::object();
        j["cx"] = x.getCx();
        j["cy"] = x.getCy();
    }

    inline void from_json(const json & j, IntGridValueInstance& x) {
        x.setCoordId(j.at("coordId").get<int64_t>());
        x.setV(j.at("v").get<int64_t>());
    }

    inline void to_json(json & j, const IntGridValueInstance & x) {
        j = json::object();
        j["coordId"] = x.getCoordId();
        j["v"] = x.getV();
    }

    inline void from_json(const json & j, TileInstance& x) {
        x.setA(j.at("a").get<double>());
        x.setD(j.at("d").get<std::vector<int64_t>>());
        x.setF(j.at("f").get<int64_t>());
        x.setPx(j.at("px").get<std::vector<int64_t>>());
        x.setSrc(j.at("src").get<std::vector<int64_t>>());
        x.setT(j.at("t").get<int64_t>());
    }

    inline void to_json(json & j, const TileInstance & x) {
        j = json::object();
        j["a"] = x.getA();
        j["d"] = x.getD();
        j["f"] = x.getF();
        j["px"] = x.getPx();
        j["src"] = x.getSrc();
        j["t"] = x.getT();
    }

    inline void from_json(const json & j, LayerInstance& x) {
        x.setCHei(j.at("__cHei").get<int64_t>());
        x.setCWid(j.at("__cWid").get<int64_t>());
        x.setGridSize(j.at("__gridSize").get<int64_t>());
        x.setIdentifier(j.at("__identifier").get<std::string>());
        x.setOpacity(j.at("__opacity").get<double>());
        x.setPxTotalOffsetX(j.at("__pxTotalOffsetX").get<int64_t>());
        x.setPxTotalOffsetY(j.at("__pxTotalOffsetY").get<int64_t>());
        x.setTilesetDefUid(get_stack_optional<int64_t>(j, "__tilesetDefUid"));
        x.setTilesetRelPath(get_stack_optional<std::string>(j, "__tilesetRelPath"));
        x.setType(j.at("__type").get<std::string>());
        x.setAutoLayerTiles(j.at("autoLayerTiles").get<std::vector<TileInstance>>());
        x.setEntityInstances(j.at("entityInstances").get<std::vector<EntityInstance>>());
        x.setGridTiles(j.at("gridTiles").get<std::vector<TileInstance>>());
        x.setIid(j.at("iid").get<std::string>());
        x.setIntGrid(get_stack_optional<std::vector<IntGridValueInstance>>(j, "intGrid"));
        x.setIntGridCsv(j.at("intGridCsv").get<std::vector<int64_t>>());
        x.setLayerDefUid(j.at("layerDefUid").get<int64_t>());
        x.setLevelId(j.at("levelId").get<int64_t>());
        x.setOptionalRules(j.at("optionalRules").get<std::vector<int64_t>>());
        x.setOverrideTilesetUid(get_stack_optional<int64_t>(j, "overrideTilesetUid"));
        x.setPxOffsetX(j.at("pxOffsetX").get<int64_t>());
        x.setPxOffsetY(j.at("pxOffsetY").get<int64_t>());
        x.setSeed(j.at("seed").get<int64_t>());
        x.setVisible(j.at("visible").get<bool>());
    }

    inline void to_json(json & j, const LayerInstance & x) {
        j = json::object();
        j["__cHei"] = x.getCHei();
        j["__cWid"] = x.getCWid();
        j["__gridSize"] = x.getGridSize();
        j["__identifier"] = x.getIdentifier();
        j["__opacity"] = x.getOpacity();
        j["__pxTotalOffsetX"] = x.getPxTotalOffsetX();
        j["__pxTotalOffsetY"] = x.getPxTotalOffsetY();
        j["__tilesetDefUid"] = x.getTilesetDefUid();
        j["__tilesetRelPath"] = x.getTilesetRelPath();
        j["__type"] = x.getType();
        j["autoLayerTiles"] = x.getAutoLayerTiles();
        j["entityInstances"] = x.getEntityInstances();
        j["gridTiles"] = x.getGridTiles();
        j["iid"] = x.getIid();
        j["intGrid"] = x.getIntGrid();
        j["intGridCsv"] = x.getIntGridCsv();
        j["layerDefUid"] = x.getLayerDefUid();
        j["levelId"] = x.getLevelId();
        j["optionalRules"] = x.getOptionalRules();
        j["overrideTilesetUid"] = x.getOverrideTilesetUid();
        j["pxOffsetX"] = x.getPxOffsetX();
        j["pxOffsetY"] = x.getPxOffsetY();
        j["seed"] = x.getSeed();
        j["visible"] = x.getVisible();
    }

    inline void from_json(const json & j, LevelBackgroundPosition& x) {
        x.setCropRect(j.at("cropRect").get<std::vector<double>>());
        x.setScale(j.at("scale").get<std::vector<double>>());
        x.setTopLeftPx(j.at("topLeftPx").get<std::vector<int64_t>>());
    }

    inline void to_json(json & j, const LevelBackgroundPosition & x) {
        j = json::object();
        j["cropRect"] = x.getCropRect();
        j["scale"] = x.getScale();
        j["topLeftPx"] = x.getTopLeftPx();
    }

    inline void from_json(const json & j, NeighbourLevel& x) {
        x.setDir(j.at("dir").get<std::string>());
        x.setLevelIid(j.at("levelIid").get<std::string>());
        x.setLevelUid(get_stack_optional<int64_t>(j, "levelUid"));
    }

    inline void to_json(json & j, const NeighbourLevel & x) {
        j = json::object();
        j["dir"] = x.getDir();
        j["levelIid"] = x.getLevelIid();
        j["levelUid"] = x.getLevelUid();
    }

    inline void from_json(const json & j, Level& x) {
        x.setBgColor(j.at("__bgColor").get<std::string>());
        x.setBgPos(get_stack_optional<LevelBackgroundPosition>(j, "__bgPos"));
        x.setNeighbours(j.at("__neighbours").get<std::vector<NeighbourLevel>>());
        x.setSmartColor(j.at("__smartColor").get<std::string>());
        x.setLevelBgColor(get_stack_optional<std::string>(j, "bgColor"));
        x.setBgPivotX(j.at("bgPivotX").get<double>());
        x.setBgPivotY(j.at("bgPivotY").get<double>());
        x.setLevelBgPos(get_stack_optional<BgPos>(j, "bgPos"));
        x.setBgRelPath(get_stack_optional<std::string>(j, "bgRelPath"));
        x.setExternalRelPath(get_stack_optional<std::string>(j, "externalRelPath"));
        x.setFieldInstances(j.at("fieldInstances").get<std::vector<FieldInstance>>());
        x.setIdentifier(j.at("identifier").get<std::string>());
        x.setIid(j.at("iid").get<std::string>());
        x.setLayerInstances(get_stack_optional<std::vector<LayerInstance>>(j, "layerInstances"));
        x.setPxHei(j.at("pxHei").get<int64_t>());
        x.setPxWid(j.at("pxWid").get<int64_t>());
        x.setUid(j.at("uid").get<int64_t>());
        x.setUseAutoIdentifier(j.at("useAutoIdentifier").get<bool>());
        x.setWorldDepth(j.at("worldDepth").get<int64_t>());
        x.setWorldX(j.at("worldX").get<int64_t>());
        x.setWorldY(j.at("worldY").get<int64_t>());
    }

    inline void to_json(json & j, const Level & x) {
        j = json::object();
        j["__bgColor"] = x.getBgColor();
        j["__bgPos"] = x.getBgPos();
        j["__neighbours"] = x.getNeighbours();
        j["__smartColor"] = x.getSmartColor();
        j["bgColor"] = x.getLevelBgColor();
        j["bgPivotX"] = x.getBgPivotX();
        j["bgPivotY"] = x.getBgPivotY();
        j["bgPos"] = x.getLevelBgPos();
        j["bgRelPath"] = x.getBgRelPath();
        j["externalRelPath"] = x.getExternalRelPath();
        j["fieldInstances"] = x.getFieldInstances();
        j["identifier"] = x.getIdentifier();
        j["iid"] = x.getIid();
        j["layerInstances"] = x.getLayerInstances();
        j["pxHei"] = x.getPxHei();
        j["pxWid"] = x.getPxWid();
        j["uid"] = x.getUid();
        j["useAutoIdentifier"] = x.getUseAutoIdentifier();
        j["worldDepth"] = x.getWorldDepth();
        j["worldX"] = x.getWorldX();
        j["worldY"] = x.getWorldY();
    }

    inline void from_json(const json & j, LdtkTableOfContentEntry& x) {
        x.setIdentifier(j.at("identifier").get<std::string>());
        x.setInstances(j.at("instances").get<std::vector<ReferenceToAnEntityInstance>>());
    }

    inline void to_json(json & j, const LdtkTableOfContentEntry & x) {
        j = json::object();
        j["identifier"] = x.getIdentifier();
        j["instances"] = x.getInstances();
    }

    inline void from_json(const json & j, World& x) {
        x.setDefaultLevelHeight(j.at("defaultLevelHeight").get<int64_t>());
        x.setDefaultLevelWidth(j.at("defaultLevelWidth").get<int64_t>());
        x.setIdentifier(j.at("identifier").get<std::string>());
        x.setIid(j.at("iid").get<std::string>());
        x.setLevels(j.at("levels").get<std::vector<Level>>());
        x.setWorldGridHeight(j.at("worldGridHeight").get<int64_t>());
        x.setWorldGridWidth(j.at("worldGridWidth").get<int64_t>());
        x.setWorldLayout(get_stack_optional<WorldLayout>(j, "worldLayout"));
    }

    inline void to_json(json & j, const World & x) {
        j = json::object();
        j["defaultLevelHeight"] = x.getDefaultLevelHeight();
        j["defaultLevelWidth"] = x.getDefaultLevelWidth();
        j["identifier"] = x.getIdentifier();
        j["iid"] = x.getIid();
        j["levels"] = x.getLevels();
        j["worldGridHeight"] = x.getWorldGridHeight();
        j["worldGridWidth"] = x.getWorldGridWidth();
        j["worldLayout"] = x.getWorldLayout();
    }

    inline void from_json(const json & j, ForcedRefs& x) {
        x.setAutoLayerRuleGroup(get_stack_optional<AutoLayerRuleGroup>(j, "AutoLayerRuleGroup"));
        x.setAutoRuleDef(get_stack_optional<AutoLayerRuleDefinition>(j, "AutoRuleDef"));
        x.setCustomCommand(get_stack_optional<LdtkCustomCommand>(j, "CustomCommand"));
        x.setDefinitions(get_stack_optional<Definitions>(j, "Definitions"));
        x.setEntityDef(get_stack_optional<EntityDefinition>(j, "EntityDef"));
        x.setEntityInstance(get_stack_optional<EntityInstance>(j, "EntityInstance"));
        x.setEntityReferenceInfos(get_stack_optional<ReferenceToAnEntityInstance>(j, "EntityReferenceInfos"));
        x.setEnumDef(get_stack_optional<EnumDefinition>(j, "EnumDef"));
        x.setEnumDefValues(get_stack_optional<EnumValueDefinition>(j, "EnumDefValues"));
        x.setEnumTagValue(get_stack_optional<EnumTagValue>(j, "EnumTagValue"));
        x.setFieldDef(get_stack_optional<FieldDefinition>(j, "FieldDef"));
        x.setFieldInstance(get_stack_optional<FieldInstance>(j, "FieldInstance"));
        x.setGridPoint(get_stack_optional<GridPoint>(j, "GridPoint"));
        x.setIntGridValueDef(get_stack_optional<IntGridValueDefinition>(j, "IntGridValueDef"));
        x.setIntGridValueGroupDef(get_stack_optional<IntGridValueGroupDefinition>(j, "IntGridValueGroupDef"));
        x.setIntGridValueInstance(get_stack_optional<IntGridValueInstance>(j, "IntGridValueInstance"));
        x.setLayerDef(get_stack_optional<LayerDefinition>(j, "LayerDef"));
        x.setLayerInstance(get_stack_optional<LayerInstance>(j, "LayerInstance"));
        x.setLevel(get_stack_optional<Level>(j, "Level"));
        x.setLevelBgPosInfos(get_stack_optional<LevelBackgroundPosition>(j, "LevelBgPosInfos"));
        x.setNeighbourLevel(get_stack_optional<NeighbourLevel>(j, "NeighbourLevel"));
        x.setTableOfContentEntry(get_stack_optional<LdtkTableOfContentEntry>(j, "TableOfContentEntry"));
        x.setTile(get_stack_optional<TileInstance>(j, "Tile"));
        x.setTileCustomMetadata(get_stack_optional<TileCustomMetadata>(j, "TileCustomMetadata"));
        x.setTilesetDef(get_stack_optional<TilesetDefinition>(j, "TilesetDef"));
        x.setTilesetRect(get_stack_optional<TilesetRectangle>(j, "TilesetRect"));
        x.setWorld(get_stack_optional<World>(j, "World"));
    }

    inline void to_json(json & j, const ForcedRefs & x) {
        j = json::object();
        j["AutoLayerRuleGroup"] = x.getAutoLayerRuleGroup();
        j["AutoRuleDef"] = x.getAutoRuleDef();
        j["CustomCommand"] = x.getCustomCommand();
        j["Definitions"] = x.getDefinitions();
        j["EntityDef"] = x.getEntityDef();
        j["EntityInstance"] = x.getEntityInstance();
        j["EntityReferenceInfos"] = x.getEntityReferenceInfos();
        j["EnumDef"] = x.getEnumDef();
        j["EnumDefValues"] = x.getEnumDefValues();
        j["EnumTagValue"] = x.getEnumTagValue();
        j["FieldDef"] = x.getFieldDef();
        j["FieldInstance"] = x.getFieldInstance();
        j["GridPoint"] = x.getGridPoint();
        j["IntGridValueDef"] = x.getIntGridValueDef();
        j["IntGridValueGroupDef"] = x.getIntGridValueGroupDef();
        j["IntGridValueInstance"] = x.getIntGridValueInstance();
        j["LayerDef"] = x.getLayerDef();
        j["LayerInstance"] = x.getLayerInstance();
        j["Level"] = x.getLevel();
        j["LevelBgPosInfos"] = x.getLevelBgPosInfos();
        j["NeighbourLevel"] = x.getNeighbourLevel();
        j["TableOfContentEntry"] = x.getTableOfContentEntry();
        j["Tile"] = x.getTile();
        j["TileCustomMetadata"] = x.getTileCustomMetadata();
        j["TilesetDef"] = x.getTilesetDef();
        j["TilesetRect"] = x.getTilesetRect();
        j["World"] = x.getWorld();
    }

    inline void from_json(const json & j, Coordinate& x) {
        x.setForcedRefs(get_stack_optional<ForcedRefs>(j, "__FORCED_REFS"));
        x.setAppBuildId(j.at("appBuildId").get<double>());
        x.setBackupLimit(j.at("backupLimit").get<int64_t>());
        x.setBackupOnSave(j.at("backupOnSave").get<bool>());
        x.setBackupRelPath(get_stack_optional<std::string>(j, "backupRelPath"));
        x.setBgColor(j.at("bgColor").get<std::string>());
        x.setCustomCommands(j.at("customCommands").get<std::vector<LdtkCustomCommand>>());
        x.setDefaultEntityHeight(j.at("defaultEntityHeight").get<int64_t>());
        x.setDefaultEntityWidth(j.at("defaultEntityWidth").get<int64_t>());
        x.setDefaultGridSize(j.at("defaultGridSize").get<int64_t>());
        x.setDefaultLevelBgColor(j.at("defaultLevelBgColor").get<std::string>());
        x.setDefaultLevelHeight(get_stack_optional<int64_t>(j, "defaultLevelHeight"));
        x.setDefaultLevelWidth(get_stack_optional<int64_t>(j, "defaultLevelWidth"));
        x.setDefaultPivotX(j.at("defaultPivotX").get<double>());
        x.setDefaultPivotY(j.at("defaultPivotY").get<double>());
        x.setDefs(j.at("defs").get<Definitions>());
        x.setDummyWorldIid(j.at("dummyWorldIid").get<std::string>());
        x.setExportLevelBg(j.at("exportLevelBg").get<bool>());
        x.setExportPng(get_stack_optional<bool>(j, "exportPng"));
        x.setExportTiled(j.at("exportTiled").get<bool>());
        x.setExternalLevels(j.at("externalLevels").get<bool>());
        x.setFlags(j.at("flags").get<std::vector<Flag>>());
        x.setIdentifierStyle(j.at("identifierStyle").get<IdentifierStyle>());
        x.setIid(j.at("iid").get<std::string>());
        x.setImageExportMode(j.at("imageExportMode").get<ImageExportMode>());
        x.setJsonVersion(j.at("jsonVersion").get<std::string>());
        x.setLevelNamePattern(j.at("levelNamePattern").get<std::string>());
        x.setLevels(j.at("levels").get<std::vector<Level>>());
        x.setMinifyJson(j.at("minifyJson").get<bool>());
        x.setNextUid(j.at("nextUid").get<int64_t>());
        x.setPngFilePattern(get_stack_optional<std::string>(j, "pngFilePattern"));
        x.setSimplifiedExport(j.at("simplifiedExport").get<bool>());
        x.setToc(j.at("toc").get<std::vector<LdtkTableOfContentEntry>>());
        x.setTutorialDesc(get_stack_optional<std::string>(j, "tutorialDesc"));
        x.setWorldGridHeight(get_stack_optional<int64_t>(j, "worldGridHeight"));
        x.setWorldGridWidth(get_stack_optional<int64_t>(j, "worldGridWidth"));
        x.setWorldLayout(get_stack_optional<WorldLayout>(j, "worldLayout"));
        x.setWorlds(j.at("worlds").get<std::vector<World>>());
    }

    inline void to_json(json & j, const Coordinate & x) {
        j = json::object();
        j["__FORCED_REFS"] = x.getForcedRefs();
        j["appBuildId"] = x.getAppBuildId();
        j["backupLimit"] = x.getBackupLimit();
        j["backupOnSave"] = x.getBackupOnSave();
        j["backupRelPath"] = x.getBackupRelPath();
        j["bgColor"] = x.getBgColor();
        j["customCommands"] = x.getCustomCommands();
        j["defaultEntityHeight"] = x.getDefaultEntityHeight();
        j["defaultEntityWidth"] = x.getDefaultEntityWidth();
        j["defaultGridSize"] = x.getDefaultGridSize();
        j["defaultLevelBgColor"] = x.getDefaultLevelBgColor();
        j["defaultLevelHeight"] = x.getDefaultLevelHeight();
        j["defaultLevelWidth"] = x.getDefaultLevelWidth();
        j["defaultPivotX"] = x.getDefaultPivotX();
        j["defaultPivotY"] = x.getDefaultPivotY();
        j["defs"] = x.getDefs();
        j["dummyWorldIid"] = x.getDummyWorldIid();
        j["exportLevelBg"] = x.getExportLevelBg();
        j["exportPng"] = x.getExportPng();
        j["exportTiled"] = x.getExportTiled();
        j["externalLevels"] = x.getExternalLevels();
        j["flags"] = x.getFlags();
        j["identifierStyle"] = x.getIdentifierStyle();
        j["iid"] = x.getIid();
        j["imageExportMode"] = x.getImageExportMode();
        j["jsonVersion"] = x.getJsonVersion();
        j["levelNamePattern"] = x.getLevelNamePattern();
        j["levels"] = x.getLevels();
        j["minifyJson"] = x.getMinifyJson();
        j["nextUid"] = x.getNextUid();
        j["pngFilePattern"] = x.getPngFilePattern();
        j["simplifiedExport"] = x.getSimplifiedExport();
        j["toc"] = x.getToc();
        j["tutorialDesc"] = x.getTutorialDesc();
        j["worldGridHeight"] = x.getWorldGridHeight();
        j["worldGridWidth"] = x.getWorldGridWidth();
        j["worldLayout"] = x.getWorldLayout();
        j["worlds"] = x.getWorlds();
    }

    inline void from_json(const json & j, When & x) {
        if (j == "AfterLoad") x = When::afterLoad;
        else if (j == "AfterSave") x = When::afterSave;
        else if (j == "BeforeSave") x = When::beforeSave;
        else if (j == "Manual") x = When::manual;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const When & x) {
        switch (x) {
            case When::afterLoad: j = "AfterLoad"; break;
            case When::afterSave: j = "AfterSave"; break;
            case When::beforeSave: j = "BeforeSave"; break;
            case When::manual: j = "Manual"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, AllowedRefs & x) {
        if (j == "Any") x = AllowedRefs::any;
        else if (j == "OnlySame") x = AllowedRefs::onlySame;
        else if (j == "OnlySpecificEntity") x = AllowedRefs::onlySpecificEntity;
        else if (j == "OnlyTags") x = AllowedRefs::onlyTags;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const AllowedRefs & x) {
        switch (x) {
            case AllowedRefs::any: j = "Any"; break;
            case AllowedRefs::onlySame: j = "OnlySame"; break;
            case AllowedRefs::onlySpecificEntity: j = "OnlySpecificEntity"; break;
            case AllowedRefs::onlyTags: j = "OnlyTags"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, EditorDisplayMode & x) {
        if (j == "ArrayCountNoLabel") x = EditorDisplayMode::arrayCountNoLabel;
        else if (j == "ArrayCountWithLabel") x = EditorDisplayMode::arrayCountWithLabel;
        else if (j == "EntityTile") x = EditorDisplayMode::entityTile;
        else if (j == "Hidden") x = EditorDisplayMode::hidden;
        else if (j == "LevelTile") x = EditorDisplayMode::levelTile;
        else if (j == "NameAndValue") x = EditorDisplayMode::nameAndValue;
        else if (j == "PointPath") x = EditorDisplayMode::pointPath;
        else if (j == "PointPathLoop") x = EditorDisplayMode::pointPathLoop;
        else if (j == "PointStar") x = EditorDisplayMode::pointStar;
        else if (j == "Points") x = EditorDisplayMode::points;
        else if (j == "RadiusGrid") x = EditorDisplayMode::radiusGrid;
        else if (j == "RadiusPx") x = EditorDisplayMode::radiusPx;
        else if (j == "RefLinkBetweenCenters") x = EditorDisplayMode::refLinkBetweenCenters;
        else if (j == "RefLinkBetweenPivots") x = EditorDisplayMode::refLinkBetweenPivots;
        else if (j == "ValueOnly") x = EditorDisplayMode::valueOnly;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const EditorDisplayMode & x) {
        switch (x) {
            case EditorDisplayMode::arrayCountNoLabel: j = "ArrayCountNoLabel"; break;
            case EditorDisplayMode::arrayCountWithLabel: j = "ArrayCountWithLabel"; break;
            case EditorDisplayMode::entityTile: j = "EntityTile"; break;
            case EditorDisplayMode::hidden: j = "Hidden"; break;
            case EditorDisplayMode::levelTile: j = "LevelTile"; break;
            case EditorDisplayMode::nameAndValue: j = "NameAndValue"; break;
            case EditorDisplayMode::pointPath: j = "PointPath"; break;
            case EditorDisplayMode::pointPathLoop: j = "PointPathLoop"; break;
            case EditorDisplayMode::pointStar: j = "PointStar"; break;
            case EditorDisplayMode::points: j = "Points"; break;
            case EditorDisplayMode::radiusGrid: j = "RadiusGrid"; break;
            case EditorDisplayMode::radiusPx: j = "RadiusPx"; break;
            case EditorDisplayMode::refLinkBetweenCenters: j = "RefLinkBetweenCenters"; break;
            case EditorDisplayMode::refLinkBetweenPivots: j = "RefLinkBetweenPivots"; break;
            case EditorDisplayMode::valueOnly: j = "ValueOnly"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, EditorDisplayPos & x) {
        if (j == "Above") x = EditorDisplayPos::above;
        else if (j == "Beneath") x = EditorDisplayPos::beneath;
        else if (j == "Center") x = EditorDisplayPos::center;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const EditorDisplayPos & x) {
        switch (x) {
            case EditorDisplayPos::above: j = "Above"; break;
            case EditorDisplayPos::beneath: j = "Beneath"; break;
            case EditorDisplayPos::center: j = "Center"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, EditorLinkStyle & x) {
        if (j == "ArrowsLine") x = EditorLinkStyle::arrowsLine;
        else if (j == "CurvedArrow") x = EditorLinkStyle::curvedArrow;
        else if (j == "DashedLine") x = EditorLinkStyle::dashedLine;
        else if (j == "StraightArrow") x = EditorLinkStyle::straightArrow;
        else if (j == "ZigZag") x = EditorLinkStyle::zigZag;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const EditorLinkStyle & x) {
        switch (x) {
            case EditorLinkStyle::arrowsLine: j = "ArrowsLine"; break;
            case EditorLinkStyle::curvedArrow: j = "CurvedArrow"; break;
            case EditorLinkStyle::dashedLine: j = "DashedLine"; break;
            case EditorLinkStyle::straightArrow: j = "StraightArrow"; break;
            case EditorLinkStyle::zigZag: j = "ZigZag"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, TextLanguageMode & x) {
        if (j == "LangC") x = TextLanguageMode::langC;
        else if (j == "LangHaxe") x = TextLanguageMode::langHaxe;
        else if (j == "LangJS") x = TextLanguageMode::langJs;
        else if (j == "LangJson") x = TextLanguageMode::langJson;
        else if (j == "LangLog") x = TextLanguageMode::langLog;
        else if (j == "LangLua") x = TextLanguageMode::langLua;
        else if (j == "LangMarkdown") x = TextLanguageMode::langMarkdown;
        else if (j == "LangPython") x = TextLanguageMode::langPython;
        else if (j == "LangRuby") x = TextLanguageMode::langRuby;
        else if (j == "LangXml") x = TextLanguageMode::langXml;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const TextLanguageMode & x) {
        switch (x) {
            case TextLanguageMode::langC: j = "LangC"; break;
            case TextLanguageMode::langHaxe: j = "LangHaxe"; break;
            case TextLanguageMode::langJs: j = "LangJS"; break;
            case TextLanguageMode::langJson: j = "LangJson"; break;
            case TextLanguageMode::langLog: j = "LangLog"; break;
            case TextLanguageMode::langLua: j = "LangLua"; break;
            case TextLanguageMode::langMarkdown: j = "LangMarkdown"; break;
            case TextLanguageMode::langPython: j = "LangPython"; break;
            case TextLanguageMode::langRuby: j = "LangRuby"; break;
            case TextLanguageMode::langXml: j = "LangXml"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, LimitBehavior & x) {
        if (j == "DiscardOldOnes") x = LimitBehavior::discardOldOnes;
        else if (j == "MoveLastOne") x = LimitBehavior::moveLastOne;
        else if (j == "PreventAdding") x = LimitBehavior::preventAdding;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const LimitBehavior & x) {
        switch (x) {
            case LimitBehavior::discardOldOnes: j = "DiscardOldOnes"; break;
            case LimitBehavior::moveLastOne: j = "MoveLastOne"; break;
            case LimitBehavior::preventAdding: j = "PreventAdding"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, LimitScope & x) {
        if (j == "PerLayer") x = LimitScope::perLayer;
        else if (j == "PerLevel") x = LimitScope::perLevel;
        else if (j == "PerWorld") x = LimitScope::perWorld;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const LimitScope & x) {
        switch (x) {
            case LimitScope::perLayer: j = "PerLayer"; break;
            case LimitScope::perLevel: j = "PerLevel"; break;
            case LimitScope::perWorld: j = "PerWorld"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, RenderMode & x) {
        if (j == "Cross") x = RenderMode::cross;
        else if (j == "Ellipse") x = RenderMode::ellipse;
        else if (j == "Rectangle") x = RenderMode::rectangle;
        else if (j == "Tile") x = RenderMode::tile;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const RenderMode & x) {
        switch (x) {
            case RenderMode::cross: j = "Cross"; break;
            case RenderMode::ellipse: j = "Ellipse"; break;
            case RenderMode::rectangle: j = "Rectangle"; break;
            case RenderMode::tile: j = "Tile"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, TileRenderMode & x) {
        if (j == "Cover") x = TileRenderMode::cover;
        else if (j == "FitInside") x = TileRenderMode::fitInside;
        else if (j == "FullSizeCropped") x = TileRenderMode::fullSizeCropped;
        else if (j == "FullSizeUncropped") x = TileRenderMode::fullSizeUncropped;
        else if (j == "NineSlice") x = TileRenderMode::nineSlice;
        else if (j == "Repeat") x = TileRenderMode::repeat;
        else if (j == "Stretch") x = TileRenderMode::stretch;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const TileRenderMode & x) {
        switch (x) {
            case TileRenderMode::cover: j = "Cover"; break;
            case TileRenderMode::fitInside: j = "FitInside"; break;
            case TileRenderMode::fullSizeCropped: j = "FullSizeCropped"; break;
            case TileRenderMode::fullSizeUncropped: j = "FullSizeUncropped"; break;
            case TileRenderMode::nineSlice: j = "NineSlice"; break;
            case TileRenderMode::repeat: j = "Repeat"; break;
            case TileRenderMode::stretch: j = "Stretch"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, Checker & x) {
        if (j == "Horizontal") x = Checker::horizontal;
        else if (j == "None") x = Checker::none;
        else if (j == "Vertical") x = Checker::vertical;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const Checker & x) {
        switch (x) {
            case Checker::horizontal: j = "Horizontal"; break;
            case Checker::none: j = "None"; break;
            case Checker::vertical: j = "Vertical"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, TileMode & x) {
        if (j == "Single") x = TileMode::single;
        else if (j == "Stamp") x = TileMode::stamp;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const TileMode & x) {
        switch (x) {
            case TileMode::single: j = "Single"; break;
            case TileMode::stamp: j = "Stamp"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, Type & x) {
        if (j == "AutoLayer") x = Type::autoLayer;
        else if (j == "Entities") x = Type::entities;
        else if (j == "IntGrid") x = Type::intGrid;
        else if (j == "Tiles") x = Type::tiles;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const Type & x) {
        switch (x) {
            case Type::autoLayer: j = "AutoLayer"; break;
            case Type::entities: j = "Entities"; break;
            case Type::intGrid: j = "IntGrid"; break;
            case Type::tiles: j = "Tiles"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, EmbedAtlas & x) {
        if (j == "LdtkIcons") x = EmbedAtlas::ldtkIcons;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const EmbedAtlas & x) {
        switch (x) {
            case EmbedAtlas::ldtkIcons: j = "LdtkIcons"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, Flag & x) {
        if (j == "DiscardPreCsvIntGrid") x = Flag::discardPreCsvIntGrid;
        else if (j == "ExportPreCsvIntGridFormat") x = Flag::exportPreCsvIntGridFormat;
        else if (j == "IgnoreBackupSuggest") x = Flag::ignoreBackupSuggest;
        else if (j == "MultiWorlds") x = Flag::multiWorlds;
        else if (j == "PrependIndexToLevelFileNames") x = Flag::prependIndexToLevelFileNames;
        else if (j == "UseMultilinesType") x = Flag::useMultilinesType;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const Flag & x) {
        switch (x) {
            case Flag::discardPreCsvIntGrid: j = "DiscardPreCsvIntGrid"; break;
            case Flag::exportPreCsvIntGridFormat: j = "ExportPreCsvIntGridFormat"; break;
            case Flag::ignoreBackupSuggest: j = "IgnoreBackupSuggest"; break;
            case Flag::multiWorlds: j = "MultiWorlds"; break;
            case Flag::prependIndexToLevelFileNames: j = "PrependIndexToLevelFileNames"; break;
            case Flag::useMultilinesType: j = "UseMultilinesType"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, BgPos & x) {
        if (j == "Contain") x = BgPos::contain;
        else if (j == "Cover") x = BgPos::cover;
        else if (j == "CoverDirty") x = BgPos::coverDirty;
        else if (j == "Repeat") x = BgPos::repeat;
        else if (j == "Unscaled") x = BgPos::unscaled;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const BgPos & x) {
        switch (x) {
            case BgPos::contain: j = "Contain"; break;
            case BgPos::cover: j = "Cover"; break;
            case BgPos::coverDirty: j = "CoverDirty"; break;
            case BgPos::repeat: j = "Repeat"; break;
            case BgPos::unscaled: j = "Unscaled"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, WorldLayout & x) {
        if (j == "Free") x = WorldLayout::free;
        else if (j == "GridVania") x = WorldLayout::gridVania;
        else if (j == "LinearHorizontal") x = WorldLayout::linearHorizontal;
        else if (j == "LinearVertical") x = WorldLayout::linearVertical;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const WorldLayout & x) {
        switch (x) {
            case WorldLayout::free: j = "Free"; break;
            case WorldLayout::gridVania: j = "GridVania"; break;
            case WorldLayout::linearHorizontal: j = "LinearHorizontal"; break;
            case WorldLayout::linearVertical: j = "LinearVertical"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, IdentifierStyle & x) {
        if (j == "Capitalize") x = IdentifierStyle::capitalize;
        else if (j == "Free") x = IdentifierStyle::free;
        else if (j == "Lowercase") x = IdentifierStyle::lowercase;
        else if (j == "Uppercase") x = IdentifierStyle::uppercase;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const IdentifierStyle & x) {
        switch (x) {
            case IdentifierStyle::capitalize: j = "Capitalize"; break;
            case IdentifierStyle::free: j = "Free"; break;
            case IdentifierStyle::lowercase: j = "Lowercase"; break;
            case IdentifierStyle::uppercase: j = "Uppercase"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, ImageExportMode & x) {
        if (j == "LayersAndLevels") x = ImageExportMode::layersAndLevels;
        else if (j == "None") x = ImageExportMode::none;
        else if (j == "OneImagePerLayer") x = ImageExportMode::oneImagePerLayer;
        else if (j == "OneImagePerLevel") x = ImageExportMode::oneImagePerLevel;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const ImageExportMode & x) {
        switch (x) {
            case ImageExportMode::layersAndLevels: j = "LayersAndLevels"; break;
            case ImageExportMode::none: j = "None"; break;
            case ImageExportMode::oneImagePerLayer: j = "OneImagePerLayer"; break;
            case ImageExportMode::oneImagePerLevel: j = "OneImagePerLevel"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }
}
// NOLINTEND