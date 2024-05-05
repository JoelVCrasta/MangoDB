#include <windows.h>
#include <stdio.h>
#include <commctrl.h>
#include "includes/file_operations.c"

char usnid[5][256] = {"1", "2", "3", "4", "5"};

// Window Procedure Declaration
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SubWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Utils
WNDCLASS WindowClass();
int GetCenterX(HWND hwnd, int width);
HWND CreateButton(HWND hwnd, LPCSTR bText, int bX, int bY, int bWidth, int bHeight, int bID);

// Sub Windows
int InsertSubWindow(HWND hwnd);
int DeleteSubWindow(HWND hwnd);
int UpdateSubWindow(HWND hwnd);
int SearchSubWindow(HWND hwnd);
int DisplaySubWindow(HWND hwnd);

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {

    MSG msg; 
    HWND hwnd;
    WNDCLASS wc;

    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.lpszClassName    = "WindowClass";
    wc.hInstance        = hInstance;
    wc.hbrBackground    = GetSysColorBrush(COLOR_3DSHADOW);
    wc.lpszMenuName     = NULL;
    wc.lpfnWndProc      = WndProc;
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon            = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClass(&wc);

    hwnd = CreateWindow(wc.lpszClassName, "MangoDB", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 660 , 140, 600, 800, NULL, NULL, hInstance, NULL);

    if(hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONERROR | MB_OK);
    }

    CreateWindow("STATIC", "Welcome to MangoDB ", WS_CHILD | WS_VISIBLE | DT_CENTER, 190, 12, 200, 20, hwnd, NULL, NULL, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;    
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    switch (msg) {
        case WM_CREATE:
            int bWidth      = 400;
            int bHeight     = 100;
            int xPosition   = GetCenterX(hwnd, bWidth);
            int yPosition   = 80;

            CreateButton(hwnd, "INSERT", xPosition, yPosition , bWidth, bHeight, 1);
            CreateButton(hwnd, "DELETE", xPosition, yPosition + 120, bWidth, bHeight, 2);
            CreateButton(hwnd, "UPDATE", xPosition, yPosition + 240, bWidth, bHeight, 3);
            CreateButton(hwnd, "SEARCH", xPosition, yPosition + 360, bWidth, bHeight, 4);
            CreateButton(hwnd, "DISPLAY", xPosition, yPosition + 480, bWidth, bHeight, 5);

            break;

        case WM_COMMAND: 
            switch (LOWORD(wParam)) {
                case 1:
                    InsertSubWindow(hwnd);
                    break;
                
                case 2:
                    DeleteSubWindow(hwnd);  
                    break;

                case 3:
                    UpdateSubWindow(hwnd);
                    break;

                case 4:
                    SearchSubWindow(hwnd);
                    break;

                case 5:
                    DisplaySubWindow(hwnd);
                    break;
            }
            break;

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}


int GetCenterX(HWND hwnd, int width) {
    RECT rect;
    GetClientRect(hwnd, &rect);
    return (rect.right - width) / 2;
}

HWND CreateButton(HWND hwnd, LPCSTR bText, int bX, int bY, int bWidth, int bHeight, int bID) {
    return CreateWindow("BUTTON", bText, WS_VISIBLE | WS_CHILD, bX, bY, bWidth, bHeight, hwnd, (HMENU) IntToPtr(bID), NULL, NULL);
}


WNDCLASS WindowClass() {
    WNDCLASS swc;

    swc.style = CS_HREDRAW | CS_VREDRAW;
    swc.cbClsExtra = 0;
    swc.cbWndExtra = 0;
    swc.lpszClassName = "SubWindowClass";
    swc.hInstance = GetModuleHandle(NULL);
    swc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    swc.lpszMenuName = NULL;
    swc.lpfnWndProc = SubWndProc;
    swc.hCursor = LoadCursor(NULL, IDC_ARROW);
    swc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClass(&swc);

    return swc;
}


// ==========================================================


int InsertSubWindow(HWND hwnd) {
    EnableWindow(hwnd, FALSE);

    WNDCLASS swc = WindowClass();
    HWND ISWindow = CreateWindow(swc.lpszClassName, "INSERT", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 500, 200,  NULL, NULL, NULL, NULL);
    
    if(ISWindow == NULL) {
        MessageBox(hwnd, "Window Creation Failed!", "Error!", MB_ICONERROR | MB_OK);
    }

    SetWindowLongPtr(ISWindow, GWLP_USERDATA, (LONG_PTR) hwnd);

    // TextBox Input and Submit Button

    CreateWindow("STATIC", "USN: ", WS_CHILD | WS_VISIBLE, 60, 12, 50, 20, ISWindow, NULL, NULL, NULL);
    CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 120, 10, 300, 20, ISWindow, (HMENU)11, NULL, NULL);

    CreateWindow("STATIC", "NAME: ", WS_CHILD | WS_VISIBLE, 60, 42, 50, 20, ISWindow, NULL, NULL, NULL);
    CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 120, 40, 300, 20, ISWindow, (HMENU)12, NULL, NULL);

    CreateWindow("STATIC", "MARKS: ", WS_CHILD | WS_VISIBLE, 60, 72, 50, 20, ISWindow, NULL, NULL, NULL);
    CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 120, 70, 300, 20, ISWindow, (HMENU)13, NULL, NULL);

    HWND ISLabel = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE, 60, 102, 200, 20, ISWindow, (HMENU)14, NULL, NULL);

    CreateWindow("BUTTON", "Submit", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 340, 110, 80, 30, ISWindow, (HMENU)1, NULL, NULL);
    return 0;
}


int DeleteSubWindow(HWND hwnd) {
    EnableWindow(hwnd, FALSE);

    WNDCLASS swc = WindowClass();
    HWND DSWindow = CreateWindow(swc.lpszClassName, "DELETE", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 500, 200,  NULL, NULL, NULL, NULL);
    
    if(DSWindow == NULL) {
        MessageBox(hwnd, "Window Creation Failed!", "Error!", MB_ICONERROR | MB_OK);
    }

    SetWindowLongPtr(DSWindow, GWLP_USERDATA, (LONG_PTR) hwnd);

    CreateWindow("STATIC", "Enter the USN to remove the Record :", WS_CHILD | WS_VISIBLE, 60, 12, 400, 20, DSWindow, NULL, NULL, NULL);
    CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 60, 32, 300, 20, DSWindow, (HMENU)15, NULL, NULL);

    HWND DSLabel = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE, 60, 62, 200, 20, DSWindow, (HMENU)16, NULL, NULL);

    CreateWindow("BUTTON", "Submit", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 340, 110, 80, 30, DSWindow, (HMENU)2, NULL, NULL);

    return 0;
}


int UpdateSubWindow(HWND hwnd) {
    EnableWindow(hwnd, FALSE);

    WNDCLASS swc = WindowClass();
    HWND USWindow = CreateWindow(swc.lpszClassName, "UPDATE", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 500, 300,  NULL, NULL, NULL, NULL);
    
    if(USWindow == NULL) {
        MessageBox(hwnd, "Window Creation Failed!", "Error!", MB_ICONERROR | MB_OK);
    }

    SetWindowLongPtr(USWindow, GWLP_USERDATA, (LONG_PTR) hwnd);

    CreateWindow("STATIC", "Enter the USN to Update the Record :", WS_CHILD | WS_VISIBLE, 60, 12, 400, 20, USWindow, NULL, NULL, NULL);
    CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 60, 32, 300, 20, USWindow, (HMENU)17, NULL, NULL);

    CreateWindow("STATIC", "Enter the new Name :", WS_CHILD | WS_VISIBLE, 60, 62, 400, 20, USWindow, NULL, NULL, NULL);
    CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 60, 82, 300, 20, USWindow, (HMENU)18, NULL, NULL);

    CreateWindow("STATIC", "Enter the new Marks :", WS_CHILD | WS_VISIBLE, 60, 112, 400, 20, USWindow, NULL, NULL, NULL);
    CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 60, 132, 300, 20, USWindow, (HMENU)19, NULL, NULL);

    HWND USLabel = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE, 60, 172, 200, 20, USWindow, (HMENU)20, NULL, NULL);

    CreateWindow("BUTTON", "Submit", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 340, 210, 80, 30, USWindow, (HMENU)3, NULL, NULL);

    return 0;
}


int SearchSubWindow(HWND hwnd) {
    EnableWindow(hwnd, FALSE);

    WNDCLASS swc = WindowClass();
    HWND SSWindow = CreateWindow(swc.lpszClassName, "SEARCH", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 500, 200,  NULL, NULL, NULL, NULL);
    
    if(SSWindow == NULL) {
        MessageBox(hwnd, "Window Creation Failed!", "Error!", MB_ICONERROR | MB_OK);
    }

    SetWindowLongPtr(SSWindow, GWLP_USERDATA, (LONG_PTR) hwnd);

    CreateWindow("STATIC", "Enter the USN to Search the Record :", WS_CHILD | WS_VISIBLE, 60, 12, 400, 21, SSWindow, NULL, NULL, NULL);
    CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 60, 32, 300, 20, SSWindow, (HMENU)22, NULL, NULL);
    HWND SSLabel = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE, 60, 62, 250, 100, SSWindow, (HMENU)23, NULL, NULL);

    CreateWindow("BUTTON", "Submit", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 340, 110, 80, 30, SSWindow, (HMENU)4, NULL, NULL);

    return 0;
}


int DisplaySubWindow(HWND hwnd) {
    EnableWindow(hwnd, FALSE);

    WNDCLASS swc = WindowClass();
    HWND DiSWindow = CreateWindow(swc.lpszClassName, "DISPLAY", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 500, 800,  NULL, NULL, NULL, NULL);
    
    if(DiSWindow == NULL) {
        MessageBox(hwnd, "Window Creation Failed!", "Error!", MB_ICONERROR | MB_OK);
    }

    CreateWindow("BUTTON", "GET RECORDS", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 150, 10, 200, 50, DiSWindow, (HMENU)5, NULL, NULL);

    /* HWND DiSLabel = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE, 100, 90, 500, 1000, DiSWindow, (HMENU)24, NULL, NULL); */

    // Create the ListView control
    HWND hListView = CreateWindow(WC_LISTVIEW, "", WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_EDITLABELS, 10, 70, 460, 650, DiSWindow, (HMENU)24, NULL, NULL);

    LVCOLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

    lvc.cx = 100;
    lvc.pszText = "USN";
    ListView_InsertColumn(hListView, 0, &lvc);

    lvc.cx = 260;
    lvc.pszText = "NAME";
    ListView_InsertColumn(hListView, 1, &lvc);

    lvc.cx = 100;
    lvc.pszText = "MARKS";
    ListView_InsertColumn(hListView, 2, &lvc);

    SetWindowLongPtr(DiSWindow, GWLP_USERDATA, (LONG_PTR) hwnd);

    return 0;
}


LRESULT CALLBACK SubWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CLOSE:
            HWND hMainWnd = (HWND) GetWindowLongPtr(hwnd, GWLP_USERDATA);
            if(hMainWnd != NULL) {
                EnableWindow(hMainWnd, TRUE);
            }

            DestroyWindow(hwnd);
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case 1:
                    char i_usn[255];
                    char i_name[255];
                    char i_marks[255];

                    GetWindowText(GetDlgItem(hwnd, 11), i_usn, 255);
                    GetWindowText(GetDlgItem(hwnd, 12), i_name, 255);
                    GetWindowText(GetDlgItem(hwnd, 13), i_marks, 255);

                    if(strlen(i_usn) == 0 || strlen(i_name) == 0 || strlen(i_marks) == 0) {
                        SetWindowText(GetDlgItem(hwnd, 14), "Please fill all the fields.");
                        SetTimer(hwnd, 1, 3000, NULL);
                        break;
                    }

                    printf("%s %s %s ", i_usn, i_name, i_marks);

                    // Call the insert_record function from file_operations.c
                    int i_status = insert_record(i_usn, i_name, i_marks);

                    // Display the status message
                    if(i_status == 0) {
                        SetWindowText(GetDlgItem(hwnd, 14), "Record Inserted Successfully.");
                    } else if (i_status == 1){
                        SetWindowText(GetDlgItem(hwnd, 14), "Record Already Exists.");
                    }
                    else {
                        SetWindowText(GetDlgItem(hwnd, 14), "Error Opening File.");
                    }

                    SetWindowText(GetDlgItem(hwnd, 11), "");
                    SetWindowText(GetDlgItem(hwnd, 12), "");
                    SetWindowText(GetDlgItem(hwnd, 13), "");

                    SetTimer(hwnd, 1, 3000, NULL);
                    break;

                case 2:
                    char d_usn[255];
                    GetWindowText(GetDlgItem(hwnd, 15), d_usn, 255);
                    printf("%s\n", d_usn);

                    int d_status = delete_record(d_usn);

                    if(d_status == 0) {
                        SetWindowText(GetDlgItem(hwnd, 16), "Record Deleted Successfully.");
                    } else if (d_status == 1) {
                        SetWindowText(GetDlgItem(hwnd, 16), "Record Not Found.");
                    } else {
                        SetWindowText(GetDlgItem(hwnd, 16), "Error Creating New File.");
                    }

                    SetWindowText(GetDlgItem(hwnd, 15), "");

                    SetTimer(hwnd, 1, 3000, NULL);
                    break;

                case 3:
                    char u_usn[255];
                    char u_name[255];
                    char u_marks[255];

                    GetWindowText(GetDlgItem(hwnd, 17), u_usn, 255);
                    GetWindowText(GetDlgItem(hwnd, 18), u_name, 255);
                    GetWindowText(GetDlgItem(hwnd, 19), u_marks, 255);

                    if(strlen(u_usn) == 0 || strlen(u_name) == 0 || strlen(u_marks) == 0) {
                        SetWindowText(GetDlgItem(hwnd, 20), "Please fill all the fields.");
                        SetTimer(hwnd, 1, 3000, NULL);
                        break;
                    }

                    int u_status = update_record(u_usn, u_name, u_marks);

                    if(u_status == 0) {
                        SetWindowText(GetDlgItem(hwnd, 20), "Record Updated Successfully.");
                    } else if (u_status == 1) {
                        SetWindowText(GetDlgItem(hwnd, 20), "Record Not Found.");
                    } else {
                        SetWindowText(GetDlgItem(hwnd, 20), "Error Opening File.");
                    }

                    SetWindowText(GetDlgItem(hwnd, 17), "");
                    SetWindowText(GetDlgItem(hwnd, 18), "");
                    SetWindowText(GetDlgItem(hwnd, 19), "");

                    SetTimer(hwnd, 1, 3000, NULL);
                    break;

                case 4:
                    char s_usn[255];
                    GetWindowText(GetDlgItem(hwnd, 22), s_usn, 255);
                    printf("%s\n", s_usn);

                    char **s_record = search_record(s_usn);

                    if(s_record == NULL) {
                        SetWindowText(GetDlgItem(hwnd, 23), "Record Not Found.");
                    } else {
                        char text[255];
                        sprintf(text, "USN: %s\nName: %s\nMarks: %s", s_usn, s_record[0], s_record[1]);
                        SetWindowText(GetDlgItem(hwnd, 23), text);
                    }

                    SetWindowText(GetDlgItem(hwnd, 22), "");
                    break;

                case 5:
                        Record* d_record = display_records();
                        int count = get_record_count();

                        HWND hListView = GetDlgItem(hwnd, 24);

                        // Clear the ListView
                        ListView_DeleteAllItems(hListView);

                        // Add the records to the ListView
                        LVITEM lvi;
                        lvi.mask = LVIF_TEXT;
                        for(int i = 0; i < count; i++) {
                            lvi.iItem = i; // Row

                            lvi.iSubItem = 0;
                            lvi.pszText = d_record[i].usn;
                            ListView_InsertItem(hListView, &lvi);

                            lvi.iSubItem = 1;
                            lvi.pszText = d_record[i].name;
                            ListView_SetItem(hListView, &lvi);

                            lvi.iSubItem = 2;
                            lvi.pszText = d_record[i].marks;
                            ListView_SetItem(hListView, &lvi);
                        }

                        free(d_record);
                    break;
            }
            break;

        case WM_TIMER:
            SetWindowText(GetDlgItem(hwnd, 14), "");
            SetWindowText(GetDlgItem(hwnd, 16), "");
            SetWindowText(GetDlgItem(hwnd, 20), "");
            KillTimer(hwnd, 1);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

