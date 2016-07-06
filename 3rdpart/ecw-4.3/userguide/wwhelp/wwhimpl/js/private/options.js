// Copyright (c) 2001-2003 Quadralay Corporation.  All rights reserved.
//

function  WWHJavaScriptSettings_Object()
{
  this.mHoverText = new WWHJavaScriptSettings_HoverText_Object();

  this.mTabs      = new WWHJavaScriptSettings_Tabs_Object();
  this.mTOC       = new WWHJavaScriptSettings_TOC_Object();
  this.mIndex     = new WWHJavaScriptSettings_Index_Object();
  this.mSearch    = new WWHJavaScriptSettings_Search_Object();
  this.mFavorites = new WWHJavaScriptSettings_Favorites_Object();
}

function  WWHJavaScriptSettings_HoverText_Object()
{
  this.mbEnabled = true;

  this.mFontStyle = "font-family: Arial, Helvetica, sans-serif ; font-size: 8pt";

  this.mWidth = 150;

  this.mForegroundColor = "#000000";
  this.mBackgroundColor = "#d1e9b9";
  this.mBorderColor     = "#666666";
}

function  WWHJavaScriptSettings_Tabs_Object()
{
  this.mFontStyle = "font-family: Arial, Helvetica, sans-serif ; font-size: 9pt; font-weight: strong;";

  this.mSelectedTabForegroundColor = "#FFFFFF";

  this.mDefaultTabForegroundColor = "#666666";
}

function  WWHJavaScriptSettings_TOC_Object()
{
  this.mbShow = true;

  this.mFontStyle = "font-family: Arial, Helvetica, sans-serif ; font-size: 8pt";

  this.mHighlightColor = "#85a5b1";
  this.mEnabledColor   = "#0080B2";
  this.mDisabledColor  = "black";

  this.mIndent = 17;
}

function  WWHJavaScriptSettings_Index_Object()
{
  this.mbShow = true;

  this.mFontStyle = "font-family: Arial, Helvetica, sans-serif ; font-size: 8pt";

  this.mHighlightColor = "#85a5b1";
  this.mEnabledColor   = "#0080B2";
  this.mDisabledColor  = "black";

  this.mIndent = 17;

  this.mNavigationFontStyle      = "font-family: Arial, Helvetica, sans-serif ; font-size: 7pt ; font-weight: bold";
  this.mNavigationCurrentColor   = "black";
  this.mNavigationHighlightColor = "#85a5b1";
  this.mNavigationEnabledColor   = "#0080B2";
  this.mNavigationDisabledColor  = "#666666";
}

function  WWHJavaScriptSettings_Index_DisplayOptions(ParamIndexOptions)
{
  ParamIndexOptions.fSetThreshold(500);
  ParamIndexOptions.fSetSeperator(" - ");
}

function  WWHJavaScriptSettings_Search_Object()
{
  this.mbShow = true;

  this.mFontStyle = "font-family: Arial, Helvetica, sans-serif ; font-size: 8pt";

  this.mHighlightColor = "#85a5b1";
  this.mEnabledColor   = "#0080B2";
  this.mDisabledColor  = "black";

  this.mbResultsByBook = true;
  this.mbShowRank      = true;
}

function  WWHJavaScriptSettings_Favorites_Object()
{
  this.mbShow = true;

  this.mFontStyle = "font-family: Arial, Helvetica, sans-serif ; font-size: 8pt";

  this.mHighlightColor = "#85a5b1";
  this.mEnabledColor   = "#0080B2";
  this.mDisabledColor  = "black";
}
