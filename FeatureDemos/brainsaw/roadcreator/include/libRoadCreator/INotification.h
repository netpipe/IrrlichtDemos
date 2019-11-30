#ifndef _I_NOTIFICATION
  #define _I_NOTIFICATION

class CSegment;

/**
 * IDs for the custom messages sent by some dialogs
 */
enum eUserEvents {
  eProjectSelected=1,   /**<! a project was selected */
  eFileDialogLoad,      /**<! a file to load was selected */
  eFileDialogSave,      /**<! the file save dialog was confirmed */
  eTextureSelected      /**<! a new texture was selected */
};

/**
 * @class INotification
 * @author Christian Keimel / bulletbyte.de
 * This interface provides two callback methods that are
 * called when something changes in a segment
 * @see CConnection
 * @see CSegment::addNotify
 * @see CSegment::delNotify
 */
class INotification {
  public:
    virtual void attributesChanged(CSegment *p)=0;  /**<! the attributes of a segment changed */
    virtual void objectDeleted(CSegment *p)=0;      /**<! a segment was deleted */
};

/**
 * @class ITextureModified
 * @author Christian Keimel / bulletbyte.de
 * This interface provides a callback that is called when a texture
 * of a segment / connection is modified
 * @see CConnectionParameterWindow
 * @see CSegmentParameterWindow
 * @see addNotify
 * @see delNotify
 */
class ITextureModified {
  public:
    virtual void textureModified()=0; /**<! called when a texture parameter has changed */
};

#endif
