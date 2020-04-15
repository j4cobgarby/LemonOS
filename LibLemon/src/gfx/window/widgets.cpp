#include <gfx/window/widgets.h>

#include <string.h>
#include <stdlib.h>

#include <math.h>

#define BUTTON_COLOUR_OUTLINE_DEFAULT

void Widget::Paint(surface_t* surface){
}

void Widget::OnMouseDown(vector2i_t mousePos){
}

void Widget::OnMouseUp(vector2i_t mousePos){
}

Widget::~Widget(){}

//////////////////////////
// Text Box
//////////////////////////
TextBox::TextBox(rect_t bounds){
    this->bounds = bounds;
}

void TextBox::Paint(surface_t* surface){
    DrawRect(bounds.pos.x, bounds.pos.y, bounds.size.x, bounds.size.y, 255, 255, 255, surface);
    int xpos = 0;
    int ypos = 0;
    for(size_t i = 0; i < lineCount; i++){
        for(size_t j = 0; j < strlen(contents[i]); j++){
            xpos += DrawChar(contents[i][j], bounds.pos.x + xpos, bounds.pos.y + ypos, textColour.r, textColour.g, textColour.b, surface);

            if((xpos > (bounds.size.x - 8 - 16))){
                xpos = 0;
                ypos += 13;
            }
        }
        ypos += 13;
        xpos = 0;
        if(ypos + 12 >= bounds.size.y) break;
    }

    DrawRect(xpos + 1, ypos, 2, 12, 0, 0, 0, surface);

    DrawRect(bounds.size.x - 16, 0, 16, bounds.size.y, 240, 240, 240, surface);

    DrawRect(bounds.size.x - 14, 2, 12, 12, 128, 128, 192, surface);
    DrawRect(bounds.size.x - 14, bounds.size.y - 14, 12, 12, 128, 128, 192, surface);
}

void TextBox::LoadText(char* text){
    char* text2 = text;
    int lineCount = 0;
    int lineIndex = 0;

    while(*text2){
        if(*text2++ == '\n') lineCount++;
        text2++;
    }

    contents = (char**)malloc(sizeof(char*) * lineCount);

    char* line = strtok(text, "\n");

    contents[lineIndex] = (char*)malloc(strlen(line) + 1);
    memset(contents[lineIndex], 0, strlen(line) + 1);
    strcpy(contents[lineIndex++], line);

    while (line = strtok(NULL, "\n"))
    {
        contents[lineIndex] = (char*)malloc(strlen(line) + 1);
        strcpy(contents[lineIndex++], line);
    }
    
    this->lineCount = lineCount;
}

void TextBox::OnMouseDown(vector2i_t mousePos){
    active = true;
}

//////////////////////////
// Button
//////////////////////////
Button::Button(char* _label, rect_t _bounds){
    labelLength = strlen(_label);
    strcpy(label, _label);

    bounds = _bounds;
}

void Button::DrawButtonBorders(surface_t* surface, bool white){
    if(white){
        DrawRect(bounds.pos.x+1, bounds.pos.y, bounds.size.x - 2, 1, 250, 250, 250, surface);
        DrawRect(bounds.pos.x+1, bounds.pos.y + bounds.size.y - 1, bounds.size.x - 2, 1, 250, 250, 250, surface);
        DrawRect(bounds.pos.x, bounds.pos.y + 1, 1, bounds.size.y - 2, 250, 250, 250, surface);
        DrawRect(bounds.pos.x + bounds.size.x - 1, bounds.pos.y + 1, 1, bounds.size.y-2, 250, 250, 250, surface);
    } else {
        DrawRect(bounds.pos.x+1, bounds.pos.y, bounds.size.x - 2, 1, 96, 96, 96, surface);
        DrawRect(bounds.pos.x+1, bounds.pos.y + bounds.size.y - 1, bounds.size.x - 2, 1, 96, 96, 96, surface);
        DrawRect(bounds.pos.x, bounds.pos.y + 1, 1, bounds.size.y - 2, 96, 96, 96, surface);
        DrawRect(bounds.pos.x + bounds.size.x - 1, bounds.pos.y + 1, 1, bounds.size.y-2, 96, 96, 96, surface);
    }
}

void Button::Paint(surface_t* surface){

    if(pressed){
        DrawRect(bounds.pos.x+1, bounds.pos.y+1, bounds.size.x-2, (bounds.size.y)/2 - 1, 224/1.1, 224/1.1, 219/1.1, surface);
        DrawRect(bounds.pos.x+1, bounds.pos.y + bounds.size.y / 2, bounds.size.x-2, bounds.size.y/2-1, 224/1.1, 224/1.1, 219/1.1, surface);
        //DrawGradient(bounds.pos.x + 1, bounds.pos.y + 1, bounds.size.x - 2, bounds.size.y - 2,{250,250,250,255},{235,235,230,255},surface);

        DrawRect(bounds.pos.x+1, bounds.pos.y, bounds.size.x - 2, 1, 96, 96, 96, surface);
        DrawRect(bounds.pos.x+1, bounds.pos.y + bounds.size.y - 1, bounds.size.x - 2, 1, 96, 96, 96, surface);
        DrawRect(bounds.pos.x, bounds.pos.y + 1, 1, bounds.size.y - 2, 96, 96, 96, surface);
        DrawRect(bounds.pos.x + bounds.size.x - 1, bounds.pos.y + 1, 1, bounds.size.y-2, 96, 96, 96, surface);
    } else {
        //DrawRect(bounds.pos.x+1, bounds.pos.y+1, bounds.size.x-2, (bounds.size.y)/2 - 1, 224, 224, 219, surface);
        //DrawRect(bounds.pos.x+1, bounds.pos.y + bounds.size.y / 2, bounds.size.x-2, bounds.size.y/2-1, 224/1.1, 224/1.1, 219/1.1, surface);
        switch(style){
            case 1: // Blue
                DrawGradientVertical(bounds.pos.x + 1, bounds.pos.y + 1, bounds.size.x - 2, bounds.size.y - 2,{50,50,150,255},{45,45,130,255},surface);
                DrawButtonBorders(surface, true);
                DrawString(label, bounds.pos.x + bounds.size.x / 2 - (labelLength*8)/2, bounds.pos.y + bounds.size.y / 2 - 8, 250, 250, 250, surface);
                break;
            case 2: // Red
                DrawGradientVertical(bounds.pos.x + 1, bounds.pos.y + 1, bounds.size.x - 2, bounds.size.y - 2,{180,60,60,255},{160,55,55,255},surface);
                DrawButtonBorders(surface, true);
                DrawString(label, bounds.pos.x + bounds.size.x / 2 - (labelLength*8)/2, bounds.pos.y + bounds.size.y / 2 - 8, 250, 250, 250, surface);
                break;
            case 3: // Yellow
                DrawGradientVertical(bounds.pos.x + 1, bounds.pos.y + 1, bounds.size.x - 2, bounds.size.y - 2,{200,160,50,255},{180,140,45,255},surface);
                DrawButtonBorders(surface, true);
                DrawString(label, bounds.pos.x + bounds.size.x / 2 - (labelLength*8)/2, bounds.pos.y + bounds.size.y / 2 - 8, 250, 250, 250, surface);
                break;
            default:
                DrawGradientVertical(bounds.pos.x + 1, bounds.pos.y + 1, bounds.size.x - 2, bounds.size.y - 2,{250,250,250,255},{235,235,230,255},surface);
                DrawButtonBorders(surface, false);
                DrawString(label, bounds.pos.x + bounds.size.x / 2 - (labelLength*8)/2, bounds.pos.y + bounds.size.y / 2 - 8, 0, 0, 0, surface);
                break;
        }
        
    }
}

void Button::OnMouseDown(vector2i_t mousePos){
    pressed = true;
}

void Button::OnMouseUp(vector2i_t mousePos){
    pressed = false;
}

//////////////////////////
// Bitmap
//////////////////////////
Bitmap::Bitmap(rect_t _bounds){
    bounds = _bounds;
    surface.buffer = (uint8_t*)malloc(bounds.size.x*bounds.size.y*4);
    surface.width = bounds.size.x;
    surface.height = bounds.size.y;
}

void Bitmap::Paint(surface_t* surface){
    for(int i = 0; i < this->surface.height; i++){
        memcpy(surface->buffer + (i + bounds.pos.y) * surface->width * 4 + bounds.pos.x * 4, this->surface.buffer + bounds.size.x * i * 4, bounds.size.x * 4);
    }
}

//////////////////////////
// Label
//////////////////////////
Label::Label(char* _label, rect_t _bounds){
    labelLength = strlen(_label) + 1;
    label = (char*)malloc(labelLength);
    strcpy(label, _label);

    bounds = _bounds;
}

void Label::Paint(surface_t* surface){
    DrawString(label, bounds.pos.x, bounds.pos.y, 0, 0, 0, surface);
}

//////////////////////////
// Scroll View
//////////////////////////
ScrollView::ScrollView(rect_t _bounds){
    bounds = _bounds;
}

void ScrollView::Paint(surface_t* surface){
    for(int i = 0; i < contents.get_length(); i++){
        rect_t _bounds = contents[i]->bounds;
        contents[i]->bounds = {_bounds.pos + _bounds.pos, _bounds.size};

        contents[i]->Paint(surface);
        contents[i]->bounds = _bounds;
    }

    DrawRect(bounds.pos.x + bounds.size.x - 24, bounds.pos.y, 24, bounds.size.y, {120, 120, 110, 255}, surface);
}

void ScrollView::OnMouseDown(vector2i_t mousePos){

}

void ScrollView::OnMouseUp(vector2i_t mousePos){
    
}

//////////////////////////
// ListView
//////////////////////////
ListItem::ListItem(char* _text){
    text = (char*)malloc(strlen(_text) + 1);//new char[strlen(_text) + 1];
    strcpy(text, _text);
}

ListView::ListView(rect_t _bounds, int itemHeight){
    bounds = _bounds;
    iBounds = bounds;
    this->itemHeight = itemHeight;
}

ListView::~ListView(){
}

void ListView::ResetScrollBar(){
    double iCount = contents.get_length();
    
    scrollMax = floor((iCount * itemHeight - iBounds.size.y) / 2);
    scrollIncrementPixels = (iBounds.size.y / 2) / floor((iCount * itemHeight - iBounds.size.y) / 2);
    scrollBarHeight = iBounds.size.y - scrollMax;

    scrollPos = 0;
}

void ListView::Paint(surface_t* surface){
    //DrawRect(bounds.pos.x, bounds.pos.y, bounds.size.x - 20, bounds.size.y, {250, 250, 250,}, surface);

    for(int i = 0; i < contents.get_length(); i++){
        ListItem* item = contents.get_at(i);

        if(i*itemHeight + 2 - scrollPos < iBounds.pos.y) continue;

        if(selected == i){
            DrawRect(iBounds.pos.x + 2,iBounds.pos.y + i*itemHeight + 2 - scrollPos, iBounds.size.x - 24, itemHeight - 4, {165,/*24*/32,24}, surface);
        }
        DrawString(item->text, iBounds.pos.x + 4, iBounds.pos.y + i * itemHeight + (itemHeight / 2) - 6 - (scrollPos * scrollIncrementPixels * 2), 0, 0, 0, surface);
    }

    DrawRect(iBounds.pos.x + iBounds.size.x - 20, iBounds.pos.y, 20, iBounds.size.y, {120, 120, 110}, surface); 
    
    DrawGradientVertical(iBounds.pos.x + 1 + iBounds.size.x - 20, iBounds.pos.y + 1 + scrollPos, 18, scrollBarHeight,{250,250,250,255},{235,235,230,255},surface);
}

void ListView::OnMouseDown(vector2i_t mousePos){
    if(mousePos.x > iBounds.pos.x + iBounds.size.x - 20){
        drag = true;
        dragPos = mousePos;

        return;
    }
    selected = floor(((double)mousePos.y + scrollPos - iBounds.pos.y) / itemHeight);

    if(selected >= contents.get_length()) selected = contents.get_length();
}

void ListView::OnMouseUp(vector2i_t mousePos){
    if(drag){
        drag = false;
        //scrollPos = mousePos.y;//+= mousePos.y - dragPos.y;
        if(!scrollPos) scrollPos = scrollMax;
        else scrollPos = 0;
        if(scrollPos < 0) scrollPos = 0;
        else if (scrollPos > scrollMax) scrollPos = scrollMax;
    }
}

void ListItem::OnMouseDown(vector2i_t mousePos){
}

void ListItem::OnMouseUp(vector2i_t mousePos){
    
}

//////////////////////////
// FileView
//////////////////////////
FileView::FileView(rect_t _bounds) : ListView(_bounds){
    bounds = _bounds;

    bounds.pos.y += pathBoxHeight;
    iBounds.size.y -= pathBoxHeight;
}

FileView::~FileView(){
}

void FileView::Paint(surface_t* surface){
    ListView::Paint(surface);
}

void FileView::OnMouseDown(vector2i_t mousePos){
    if(mousePos.y - bounds.pos.y > pathBoxHeight){
        ListView::OnMouseDown(mousePos);
    }
}

void FileView::OnMouseUp(vector2i_t mousePos){
    if(mousePos.y - bounds.pos.y > pathBoxHeight){
        FileView::OnMouseDown(mousePos);
    }
}