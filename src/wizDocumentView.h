#ifndef CORE_WIZDOCUMENTVIEW_H
#define CORE_WIZDOCUMENTVIEW_H

#include "share/wizobject.h"
#include <QSharedPointer>
#include <QWidget>

class QScrollArea;
class QLineEdit;
class QLabel;


struct WIZDOCUMENTDATA;
struct WIZDOCUMENTATTACHMENTDATA;
class CWizExplorerApp;
class CWizDatabaseManager;
class CWizUserSettings;
class CWizScrollBar;
class CWizDocumentWebView;
class CWizDatabase;
class CWizSplitter;
class CWizUserCipherForm;
class CWizObjectDownloaderHost;
class QStackedWidget;
class QWebFrame;
class QWebEnginePage;
class WizWebEngineView;
class CWizDocumentEditStatusSyncThread;
class CWizDocumentStatusChecker;
class CWizLocalProgressWebView;
class CWizDocumentTransitionView;

class TitleBar;
class EditorToolBar;
class CWizTagBar;

class CWizDocumentView : public QWidget
{
    Q_OBJECT

public:
    CWizDocumentView(CWizExplorerApp& app, QWidget* parent = 0);
    ~CWizDocumentView();
    virtual QSize sizeHint() const;
    void setSizeHint(QSize size);

    QWidget* client() const;
    CWizDocumentWebView* web() const { return m_web; }
    WizWebEngineView* commentView() const;
    CWizLocalProgressWebView* commentWidget() const;
    //
    CWizDocumentTransitionView* transitionView();
    //
    TitleBar* titleBar();
    //
    void waitForDone();

protected:
    CWizExplorerApp& m_app;
    CWizDatabaseManager& m_dbMgr;
    CWizUserSettings& m_userSettings;
    CWizObjectDownloaderHost* m_downloaderHost;
    CWizDocumentTransitionView* m_transitionView;

    QStackedWidget* m_tab;
    QWidget* m_msgWidget;
    QLabel* m_msgLabel;

    QWidget* m_docView;
    CWizDocumentWebView* m_web;
    WizWebEngineView* m_comments;
    CWizLocalProgressWebView* m_commentWidget;
    CWizSplitter* m_splitter;
    TitleBar* m_title;
    QWidget* m_blankView;

    CWizUserCipherForm* m_passwordView;
    CWizDocumentEditStatusSyncThread* m_editStatusSyncThread;
//    CWizDocumentStatusCheckThread* m_editStatusCheckThread;
    CWizDocumentStatusChecker* m_editStatusChecker;

    virtual void showEvent(QShowEvent *event);
    virtual void resizeEvent(QResizeEvent* ev);

private:
    WIZDOCUMENTDATA m_note;
    bool m_bLocked; // note is force locked as readonly status
    WizEditorMode m_editorMode;
    WizDocumentViewMode m_defaultViewMode; // user defined editing mode
    bool m_noteLoaded;
    //
    int m_editStatus;  // document edit or version status
    QSize m_sizeHint;

public:
    const WIZDOCUMENTDATA& note() const { return m_note; }
    bool isLocked() const { return m_bLocked; }
    bool isEditing() const { return m_editorMode == modeEditor; }
    WizEditorMode editorMode() const { return m_editorMode; }
    bool reload();
    void reloadNote();
    void setEditorFocus();
    bool noteLoaded() const { return m_noteLoaded; }

    void initStat(const WIZDOCUMENTDATA& data, bool forceEdit);
    void viewNote(const WIZDOCUMENTDATA& data, bool forceEdit);
    void reviewCurrentNote();
    void setEditorMode(WizEditorMode editorMode);
    void setDefaultViewMode(WizDocumentViewMode mode);
    void setModified(bool modified);
    void settingsChanged();
    void sendDocumentSavedSignal(const QString& strGUID, const QString& strKbGUID);
    void resetTitle(const QString& strTitle);
    void promptMessage(const QString& strMsg);
    bool checkListClickable();
    void setStatusToEditingByCheckList();
    //
    void showCoachingTips();

signals:
    void documentSaved(const QString& strGUID, CWizDocumentView* viewer);
    void checkDocumentEditStatusRequest(const QString& strKbGUID, const QString& strGUID);
    void stopCheckDocumentEditStatusRequest(const QString& strKbGUID, const QString& strGUID);

public Q_SLOTS:
    void onViewNoteRequested(CWizDocumentView* view, const WIZDOCUMENTDATA& doc, bool forceEditing);
    void onViewNoteLoaded(CWizDocumentView*,const WIZDOCUMENTDATA&,bool);
    void onCloseNoteRequested(CWizDocumentView* view);

    void onCipherCheckRequest();

    void on_download_finished(const WIZOBJECTDATA& data, bool bSucceed);

    void on_document_modified(const WIZDOCUMENTDATA& documentOld,
                              const WIZDOCUMENTDATA& documentNew);
    void on_document_data_modified(const WIZDOCUMENTDATA& data);
    void on_document_data_changed(const QString& strGUID, CWizDocumentView* viewer);

    void on_attachment_created(const WIZDOCUMENTATTACHMENTDATA& attachment);
    void on_attachment_deleted(const WIZDOCUMENTATTACHMENTDATA& attachment);

    //
    void on_checkEditStatus_finished(const QString& strGUID, bool editable);
    void on_checkEditStatus_timeout(const QString& strGUID);
    void on_documentEditingByOthers(QString strGUID, QStringList editors);
    void on_checkDocumentChanged_finished(const QString& strGUID, bool changed);
    void on_syncDatabase_request(const QString& strKbGUID);
    void on_webView_focus_changed();

    void on_notifyBar_link_clicked(const QString& link);

    void on_loadComment_request(const QString& url);

    void on_commentWidget_statusChanged();

private:
    void loadNote(const WIZDOCUMENTDATA &doc);
    void downloadNoteFromServer(const WIZDOCUMENTDATA& note);
    void sendDocumentEditingStatus();
    void stopDocumentEditingStatus();
    void startCheckDocumentEditStatus();
    void stopCheckDocumentEditStatus();
    bool checkDocumentEditable();
    //
    void stopCheckDocumentAnimations();    
};


#endif // CORE_WIZDOCUMENTVIEW_H
