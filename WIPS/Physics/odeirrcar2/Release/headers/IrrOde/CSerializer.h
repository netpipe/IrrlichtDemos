#ifndef _C_STACKED_BUFFER
  #define _C_STACKED_BUFFER

  #include <irrlicht.h>

namespace irr {
namespace ode {

  using namespace core;

/**
 * @class CSerializer
 * This class is used to store changes in the world with the CIrrOdeRecorder. Data added to the buffer is saved at
 * the end of the buffer, it is a "First in-First out" buffer. Be sure to get your data out of the buffer in the
 * same order as you put it in.
 * @author Christian Keimel / dustbin::games
 * @see CMessage
 * @see CIrrOdeRecorder
 * @see CIrrOdeRePlayer
 */
class CSerializer {
  protected:
    c8  m_sBuffer[4096];		/**< the buffer */
    c8 *m_pBufferPos;				/**< the current position in the buffer */
    s32 m_iCnt,							/**< the number of elements in the buffer */
		    m_iSizeOf;					/**< the size of the buffer */

		/**
		 * recalculate the buffer size
		 */
    void recalcSize() {
      m_iSizeOf=(int)(m_pBufferPos-m_sBuffer);
    }

  public:
    CSerializer() {
      m_pBufferPos=m_sBuffer;
      m_iCnt=0;
    }

    CSerializer(CSerializer *toCopy, bool bRecalcSize) {
      setBuffer(toCopy->getBuffer(),toCopy->getBufferSize(),bRecalcSize);
    }

		/**
		 * Set the buffer. Can be used to copy the content.
		 * @param pBuffer the buffer to be used
		 * @param iBufferSize the size of the buffer to be used
		 */
    void setBuffer(const c8 *pBuffer, u32 iBufferSize, bool bRecalcSize=true) {
      memcpy(m_sBuffer,pBuffer,iBufferSize);
      m_iCnt=0;
      if (bRecalcSize) {
        m_pBufferPos=m_sBuffer+iBufferSize;
        recalcSize();
      }
      else m_pBufferPos=m_sBuffer;
    }

		/**
		 * add an u8
		 * @param c u8 to add
		 */
    void addU8(u8 c) {
      *((u8 *)m_pBufferPos)=c;
      m_pBufferPos+=sizeof(u8);
      recalcSize();
      m_iCnt++;
    }

    void addS8(s8 c) {
      *((s8 *)m_pBufferPos)=c;
      m_pBufferPos+=sizeof(s8);
      recalcSize();
      m_iCnt++;
    }

		/**
		 * add a s16
		 * @param i s16 to add
		 */
    void addS16(s16 i) {
      *((s16 *)m_pBufferPos)=i;
      m_pBufferPos+=sizeof(short);
      recalcSize();
      m_iCnt++;
    }

		/**
		 * add an u16
		 * @param i u16 to add
		 */
    void addU16(u16 i) {
      *((u16 *)m_pBufferPos)=i;
      m_pBufferPos+=sizeof(u16);
      recalcSize();
      m_iCnt++;
    }

		/**
		 * add a s32
		 * @param i s32 to add
		 */
    void addS32(s32 i) {
      *((s32 *)m_pBufferPos)=i;
      m_pBufferPos+=sizeof(s32);
      recalcSize();
      m_iCnt++;
    }

		/**
		 * add an u32
		 * @param i u32 to add
		 */
    void addU32(u32 i) {
      *((u32 *)m_pBufferPos)=i;
      m_pBufferPos+=sizeof(u32);
      recalcSize();
      m_iCnt++;
    }

		/**
		 * add a float
		 * @param f float to add
		 */
    void addF32(f32 f) {
      *((f32 *)m_pBufferPos)=f;
      m_pBufferPos+=sizeof(f32);
      recalcSize();
      m_iCnt++;
    }

		/**
		 * add a vector3df
		 * @param v vector3df to add
		 */
    void addVector3df(vector3df v) {
      addF32(v.X);
      addF32(v.Y);
      addF32(v.Z);
    }

		/**
		 * add a string
		 * @param s string to add
		 */
    void addString(const c8 *s) {
      strcpy(m_pBufferPos,s);
      m_pBufferPos+=strlen(s)+1;
      recalcSize();
      m_iCnt++;
    }

		/**
		 * Reset the buffer position. The position is used for both writing and reading
		 */
    void resetBufferPos() {
      m_pBufferPos=m_sBuffer;
    }

		/**
		 * reset the buffer. Resets buffer pos and sets size to "0"
		 */
    void reset() {
      resetBufferPos();
      recalcSize();
    }

		/**
		 * Get buffer size
		 * @return the buffer size
		 */
    int getBufferSize() {
      return m_iSizeOf;
    }

		/**
		 * get an u8
		 * @return an u8
		 */
    u8 getU8() {
      u8 iRet=*((u8 *)m_pBufferPos);
      m_pBufferPos+=sizeof(u8);
      m_iCnt--;
      return iRet;
    }

    /**
     * get a s8
     * @return a s8
     */
    s8 getS8() {
      s8 iRet=*((s8 *)m_pBufferPos);
      m_pBufferPos+=sizeof(s8);
      m_iCnt--;
      return iRet;
    }

		/**
		 * get a s16
		 * @return a s16
		 */
    s16 getS16() {
      s16 iRet=*((s16 *)m_pBufferPos);
      m_pBufferPos+=sizeof(s16);
      m_iCnt--;
      return iRet;
    }

		/**
		 * get an u16
		 * @return an u16
		 */
    u16 getU16() {
      u16 iRet=*((u16 *)m_pBufferPos);
      m_pBufferPos+=sizeof(u16);
      m_iCnt--;
      return iRet;
    }

		/**
		 * get a s32
		 * @return a s32
		 */
    s32 getS32() {
      s32 iRet=*((s32 *)m_pBufferPos);
      m_pBufferPos+=sizeof(s32);
      m_iCnt--;
      return iRet;
    }

		/**
		 * get an u32
		 * @return an u32
		 */
    u32 getU32() {
      u32 iRet=*((u32 *)m_pBufferPos);
      m_pBufferPos+=sizeof(u32);
      m_iCnt--;
      return iRet;
    }

		/**
		 * get a float
		 * @return a float
		 */
    f32 getF32() {
      f32 fRet=*((f32 *)m_pBufferPos);
      m_pBufferPos+=sizeof(f32);
      m_iCnt--;
      return fRet;
    }

		/**
		 * get a vector3df
		 * @param v buffer for the vector. Value will be copied there
		 * @return a vector3df (a.k.a. "param v")
		 */
    vector3df &getVector3df(vector3df &v) {
      v.X=getF32();
      v.Y=getF32();
      v.Z=getF32();

      return v;
    }

		/**
		 * get a string
		 * @param s buffer for the string
		 * @return a string (a.k.a. "param s")
		 */
    c8 *getString(c8 *s) {
      strcpy(s,m_pBufferPos);
      m_pBufferPos+=strlen(s)+1;
      m_iCnt--;
      return s;
    }

    c8 *getString(c8 *s, u32 iMaxBytes) {
      if (strlen(m_pBufferPos)>=iMaxBytes) {
        printf("max bytes exceeded!\n");
        memcpy(s,m_pBufferPos,iMaxBytes);
        s[iMaxBytes]='\0';
        m_pBufferPos+=iMaxBytes;
        return s;
      }
      else return getString(s);
    }

		/**
		 * get the buffer's buffer
		 * @return buffer's buffer
		 */
    const c8 *getBuffer() {
      return m_sBuffer;
    }

		/**
		 * Does this message have more data?
		 * @return true or false
		 */
    bool hasMoreData() {
      return m_iCnt!=0;
    }
};

} }
#endif
