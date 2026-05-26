#ifndef _KARIN_OCCLUSIONTEST_H
#define _KARIN_OCCLUSIONTEST_H

class rvmOcclusionQuery;
class idRenderWorldLocal;

class sdOcclusionTestLocal : public sdOcclusionTest
{
	public:
							sdOcclusionTestLocal(void);
	virtual					~sdOcclusionTestLocal(void);
	bool					IsVisible(void);
	int						CountVisible(void);
	virtual void			UpdateOcclusionTest( const occlusionTest_t *def );
	virtual void			FreeOcclusionTest(void);
	void					Render(void); // backend
	void					Ready(void); // frontend
	int						GetViewID(void) const {
		return parms.view;
	}

	private:
	bool					CreateQuery(void);
	void					UpdateTri(void);
	void					MakeModelMatrix(void);
	bool					ReadResults(void);
	enum {
		DIRTY_NONE = 0,
		DIRTY_BOUNDS = 1,
		DIRTY_MATRIX = 1 << 1,
	};

public:
	int						index;
	idRenderWorldLocal		*world;
	occlusionTest_t			parms; // frontend

	private:
	rvmOcclusionQuery		*query;
	int						lastResult;
	struct srfTriangles_t	*tri; // backend
	float					modelMatrix[16]; // backend
	int						dirty;
};

class sdOcclusionQueryWrapper
{
	public:
	void					Begin();
	void					Render(idList<sdOcclusionTestLocal *> &list);
	void					End();
};

#endif
