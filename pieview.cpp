#include "pieview.h"
#include <QPainter>

PieView::PieView(QWidget *parent) :QAbstractItemView(parent)
{

}
PieView::~PieView()
{

}
void PieView::paintEvent(QPaintEvent *)
{
	QPainter painter(viewport());
	painter.setPen(Qt::black);
	int x0 = 30;
	int y0 = 280;
	//y坐标轴
	painter.drawLine(x0, y0, 30, 32);
	painter.drawLine(28, 34, 30, 32);
	painter.drawLine(30, 32, 32, 34);
	painter.drawText(20, 30, tr("销售数量"));
	for (int i = 1; i < 5;i++)
	{
		painter.drawLine(-1, -i * 50, 1, -i * 50);
		painter.drawText(-20, -i * 50, tr("%1").arg(i * 5));
	}
	//x坐标轴
	painter.drawLine(x0, y0, 378, 280);
	painter.drawLine(376, 278, 378, 280);
	painter.drawLine(378, 280, 376, 282);
	painter.drawText(380, 280, tr("品牌"));

	int pos = x0 + 20;
	int row;
	for (row = 0; row < model()->rowCount(rootIndex());row++)
	{
		QModelIndex index = model()->index(row, 0, rootIndex());
		QString dep = model()->data(index).toString();

		painter.drawText(pos, y0 + 20, dep);
		pos += 50;
	}
	int posN = x0 + 20;
	for (row = 0; row < model()->rowCount(rootIndex());row++)
	{
		//获得销售数量
		QModelIndex index = model()->index(row, 1, rootIndex());
		int sell = model()->data(index).toDouble();
		
		int width = 10;
		//颜色
		QModelIndex colorIndex = model()->index(row, 0, rootIndex());
		QColor color = QColor(model()->data(colorIndex, Qt::DecorationRole).toString());
		if (selections->isSelected(index))
		{
			painter.setBrush(QBrush(color, Qt::Dense3Pattern));
		} 
		else
		{
			painter.setBrush(QBrush(color));
		}
		painter.drawRect(QRect(posN, y0 - sell * 10, width, sell * 10));
		QRegion regionM(posN, y0 - sell * 10, width, sell * 10);
		RegionList << regionM;
		
		posN += 50;
	}

}

//为selections赋初值
void PieView::setSelectionModel(QItemSelectionModel *selectionModel)
{
	selections = selectionModel;
}

QRegion PieView::itemRegion(QModelIndex index)
{
	QRegion region;
	if (index.column() == 1)			//销售数量
	{
		region = RegionList[index.row()];
	}
	return region;
}

QRect PieView::visualRect(const QModelIndex &index) const{ QRect qr; return qr; }
void PieView::scrollTo(const QModelIndex &index, ScrollHint hint){}
QModelIndex PieView::indexAt(const QPoint &point) const
{
	QPoint newPoint(point.x(), point.y());
	QRegion region;
	foreach (region ,RegionList)
	{
		if (region.contains(newPoint))
		{
			int row = RegionList.indexOf(region);
			QModelIndex index = model()->index(row, 1,  rootIndex());
			return index;
		}
	}
	return QModelIndex();
}

QModelIndex PieView::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
	QModelIndex qm;
	return qm;
}

int PieView::horizontalOffset() const
{
	return 0;
}

int PieView::verticalOffset() const
{
	return 0;
}

bool PieView::isIndexHidden(const QModelIndex &index) const
{
	return true;
}

void PieView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command)
{

}
QRegion PieView::visualRegionForSelection(const QItemSelection &selection) const
{
	QRegion qr;
	return qr;
}